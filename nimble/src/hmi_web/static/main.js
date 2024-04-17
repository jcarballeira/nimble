import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.132.2/build/three.module.js";

//! Error with the import so added the class manually, collapse the class
class STLLoader extends THREE.Loader {
  constructor(manager) {
    super(manager);
  }

  load(url, onLoad, onProgress, onError) {
    const scope = this;
    const loader = new THREE.FileLoader(this.manager);
    loader.setPath(this.path);
    loader.setResponseType("arraybuffer");
    loader.setRequestHeader(this.requestHeader);
    loader.setWithCredentials(this.withCredentials);
    loader.load(
      url,
      function (text) {
        try {
          onLoad(scope.parse(text));
        } catch (e) {
          if (onError) {
            onError(e);
          } else {
            console.error(e);
          }

          scope.manager.itemError(url);
        }
      },
      onProgress,
      onError
    );
  }

  parse(data) {
    function isBinary(data) {
      const reader = new DataView(data);
      const face_size = (32 / 8) * 3 + (32 / 8) * 3 * 3 + 16 / 8;
      const n_faces = reader.getUint32(80, true);
      const expect = 80 + 32 / 8 + n_faces * face_size;

      if (expect === reader.byteLength) {
        return true;
      } // An ASCII STL data must begin with 'solid ' as the first six bytes.
      // However, ASCII STLs lacking the SPACE after the 'd' are known to be
      // plentiful.  So, check the first 5 bytes for 'solid'.
      // Several encodings, such as UTF-8, precede the text with up to 5 bytes:
      // https://en.wikipedia.org/wiki/Byte_order_mark#Byte_order_marks_by_encoding
      // Search for "solid" to start anywhere after those prefixes.
      // US-ASCII ordinal values for 's', 'o', 'l', 'i', 'd'

      const solid = [115, 111, 108, 105, 100];

      for (let off = 0; off < 5; off++) {
        // If "solid" text is matched to the current offset, declare it to be an ASCII STL.
        if (matchDataViewAt(solid, reader, off)) return false;
      } // Couldn't find "solid" text at the beginning; it is binary STL.

      return true;
    }

    function matchDataViewAt(query, reader, offset) {
      // Check if each byte in query matches the corresponding byte from the current offset
      for (let i = 0, il = query.length; i < il; i++) {
        if (query[i] !== reader.getUint8(offset + i, false)) return false;
      }

      return true;
    }

    function parseBinary(data) {
      const reader = new DataView(data);
      const faces = reader.getUint32(80, true);
      let r,
        g,
        b,
        hasColors = false,
        colors;
      let defaultR, defaultG, defaultB, alpha; // process STL header
      // check for default color in header ("COLOR=rgba" sequence).

      for (let index = 0; index < 80 - 10; index++) {
        if (
          reader.getUint32(index, false) == 0x434f4c4f &&
          /*COLO*/
          reader.getUint8(index + 4) == 0x52 &&
          /*'R'*/
          reader.getUint8(index + 5) == 0x3d
          /*'='*/
        ) {
          hasColors = true;
          colors = new Float32Array(faces * 3 * 3);
          defaultR = reader.getUint8(index + 6) / 255;
          defaultG = reader.getUint8(index + 7) / 255;
          defaultB = reader.getUint8(index + 8) / 255;
          alpha = reader.getUint8(index + 9) / 255;
        }
      }

      const dataOffset = 84;
      const faceLength = 12 * 4 + 2;
      const geometry = new THREE.BufferGeometry();
      const vertices = new Float32Array(faces * 3 * 3);
      const normals = new Float32Array(faces * 3 * 3);

      for (let face = 0; face < faces; face++) {
        const start = dataOffset + face * faceLength;
        const normalX = reader.getFloat32(start, true);
        const normalY = reader.getFloat32(start + 4, true);
        const normalZ = reader.getFloat32(start + 8, true);

        if (hasColors) {
          const packedColor = reader.getUint16(start + 48, true);

          if ((packedColor & 0x8000) === 0) {
            // facet has its own unique color
            r = (packedColor & 0x1f) / 31;
            g = ((packedColor >> 5) & 0x1f) / 31;
            b = ((packedColor >> 10) & 0x1f) / 31;
          } else {
            r = defaultR;
            g = defaultG;
            b = defaultB;
          }
        }

        for (let i = 1; i <= 3; i++) {
          const vertexstart = start + i * 12;
          const componentIdx = face * 3 * 3 + (i - 1) * 3;
          vertices[componentIdx] = reader.getFloat32(vertexstart, true);
          vertices[componentIdx + 1] = reader.getFloat32(vertexstart + 4, true);
          vertices[componentIdx + 2] = reader.getFloat32(vertexstart + 8, true);
          normals[componentIdx] = normalX;
          normals[componentIdx + 1] = normalY;
          normals[componentIdx + 2] = normalZ;

          if (hasColors) {
            colors[componentIdx] = r;
            colors[componentIdx + 1] = g;
            colors[componentIdx + 2] = b;
          }
        }
      }

      geometry.setAttribute("position", new THREE.BufferAttribute(vertices, 3));
      geometry.setAttribute("normal", new THREE.BufferAttribute(normals, 3));

      if (hasColors) {
        geometry.setAttribute("color", new THREE.BufferAttribute(colors, 3));
        geometry.hasColors = true;
        geometry.alpha = alpha;
      }

      return geometry;
    }

    function parseASCII(data) {
      const geometry = new THREE.BufferGeometry();
      const patternSolid = /solid([\s\S]*?)endsolid/g;
      const patternFace = /facet([\s\S]*?)endfacet/g;
      let faceCounter = 0;
      const patternFloat = /[\s]+([+-]?(?:\d*)(?:\.\d*)?(?:[eE][+-]?\d+)?)/
        .source;
      const patternVertex = new RegExp(
        "vertex" + patternFloat + patternFloat + patternFloat,
        "g"
      );
      const patternNormal = new RegExp(
        "normal" + patternFloat + patternFloat + patternFloat,
        "g"
      );
      const vertices = [];
      const normals = [];
      const normal = new THREE.Vector3();
      let result;
      let groupCount = 0;
      let startVertex = 0;
      let endVertex = 0;

      while ((result = patternSolid.exec(data)) !== null) {
        startVertex = endVertex;
        const solid = result[0];

        while ((result = patternFace.exec(solid)) !== null) {
          let vertexCountPerFace = 0;
          let normalCountPerFace = 0;
          const text = result[0];

          while ((result = patternNormal.exec(text)) !== null) {
            normal.x = parseFloat(result[1]);
            normal.y = parseFloat(result[2]);
            normal.z = parseFloat(result[3]);
            normalCountPerFace++;
          }

          while ((result = patternVertex.exec(text)) !== null) {
            vertices.push(
              parseFloat(result[1]),
              parseFloat(result[2]),
              parseFloat(result[3])
            );
            normals.push(normal.x, normal.y, normal.z);
            vertexCountPerFace++;
            endVertex++;
          } // every face have to own ONE valid normal

          if (normalCountPerFace !== 1) {
            console.error(
              "THREE.STLLoader: Something isn't right with the normal of face number " +
                faceCounter
            );
          } // each face have to own THREE valid vertices

          if (vertexCountPerFace !== 3) {
            console.error(
              "THREE.STLLoader: Something isn't right with the vertices of face number " +
                faceCounter
            );
          }

          faceCounter++;
        }

        const start = startVertex;
        const count = endVertex - startVertex;
        geometry.addGroup(start, count, groupCount);
        groupCount++;
      }

      geometry.setAttribute(
        "position",
        new THREE.Float32BufferAttribute(vertices, 3)
      );
      geometry.setAttribute(
        "normal",
        new THREE.Float32BufferAttribute(normals, 3)
      );
      return geometry;
    }

    function ensureString(buffer) {
      if (typeof buffer !== "string") {
        return THREE.LoaderUtils.decodeText(new Uint8Array(buffer));
      }

      return buffer;
    }

    function ensureBinary(buffer) {
      if (typeof buffer === "string") {
        const array_buffer = new Uint8Array(buffer.length);

        for (let i = 0; i < buffer.length; i++) {
          array_buffer[i] = buffer.charCodeAt(i) & 0xff; // implicitly assumes little-endian
        }

        return array_buffer.buffer || array_buffer;
      } else {
        return buffer;
      }
    } // start

    const binData = ensureBinary(data);
    return isBinary(binData)
      ? parseBinary(binData)
      : parseASCII(ensureString(data));
  }
}

class ThreeDScene {
  constructor() {
    // Create a scene
    this.scene = new THREE.Scene();

    // Create a camera
    this.camera = new THREE.PerspectiveCamera(
      75,
      window.innerWidth / window.innerHeight,
      0.1,
      1000
    );

    // Camera position

    this.camera.position.x = 100;
    this.camera.position.y = 60;
    this.camera.position.z = -30;

    this.camera.lookAt(0, 0, -30);
    this.camera.rotateZ(Math.PI / 2);

    // Create a renderer
    this.renderer = new THREE.WebGLRenderer();
    this.renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(this.renderer.domElement);

    // Creates a ground
    const groundGeometry = new THREE.BoxGeometry(400, 0.1, 100);
    const groundMaterial = new THREE.MeshBasicMaterial({ color: 0x333333 });
    const ground = new THREE.Mesh(groundGeometry, groundMaterial);
    ground.position.z = -76.0;
    ground.rotateX(Math.PI / 2);

    // Adds the ground to the scene
    this.scene.add(ground);

    // Sphere for joint representation
    const radius = 1;
    const widthSegments = 32;
    const heightSegments = 32;

    const geometry = new THREE.SphereGeometry(
      radius,
      widthSegments,
      heightSegments
    );
    const material = new THREE.MeshStandardMaterial({
      color: 0xffffff,
      metalness: 1,
    });
    
    this.frame_Z = 70;
    // Cables supports to create the cables
    this.cableSupportL = new THREE.Mesh(geometry, material);
    this.cableSupportL.position.set(0, 40, this.frame_Z);
  
    this.cableSupportR = new THREE.Mesh(geometry, material);
    this.cableSupportR.position.set(0, -40, this.frame_Z);


    // Creates the joints
    var loader = new STLLoader();
    var jointData = [
      { name: 'right_knee', orientation: new THREE.Euler(0, Math.PI, 0), file: 'case'},
      { name: 'left_knee', orientation: new THREE.Euler(0, Math.PI, 0), file: 'case' },

      { name: 'left_ankle', orientation: new THREE.Euler(0, Math.PI, 0), file: 'case' },
      { name: 'right_ankle', orientation: new THREE.Euler(0, Math.PI, 0), file: 'case' },

      { name: 'left_hip', orientation: new THREE.Euler(0, Math.PI, 0), file: 'case' },
      { name: 'right_hip', orientation: new THREE.Euler(0, Math.PI, 0), file: 'case' },

      { name: 'left_toe', orientation: new THREE.Euler(0, 0, 0), file: 'sensor' },
      { name: 'right_toe', orientation: new THREE.Euler(0, 0, 0), file: 'sensor' },

      { name: 'left_heel', orientation: new THREE.Euler(0, 0, 0), file: 'sensor' },
      { name: 'right_heel', orientation: new THREE.Euler(0, 0, 0), file: 'sensor' },

      { name: 'left_pelvis', orientation: new THREE.Euler(0, 0, 0), file: 'valve' },
      { name: 'right_pelvis', orientation: new THREE.Euler(0, 0, 0), file: 'valve' },
      { name: 'base_pelvis', orientation: new THREE.Euler(0, 0, 0), file: 'plate' }
    ];
    
    // Gets the .stl and adds it
    Promise.all(jointData.map(joint => {
        return this.loadSTL(loader, `./static/models/${joint.name}_${joint.file}.stl`).then(geometry => {
          const multiplier = 0.07;
          geometry.scale(multiplier, multiplier, multiplier);
    
            var jointMesh = new THREE.Mesh(geometry, material);
            jointMesh.position.set(0, 0, 0);
            jointMesh.setRotationFromEuler(joint.orientation); // Set orientation
            this.scene.add(jointMesh);
    
            // Assign the joint mesh to its corresponding property dynamically
            this[joint.name] = jointMesh;
            this.scene.add(this[joint.name]);
        });
    })).catch(error => {
        console.error('An error occurred while loading STL files:', error);
    });

    // Creates the box
    this.createBox();

    // Add lighting
    this.addDirectionalLight(this.scene, new THREE.Vector3(10, 10, 10), 1);
    this.addDirectionalLight(this.scene, new THREE.Vector3(-10, 10, -10), 2);
    this.addDirectionalLight(this.scene, new THREE.Vector3(-10, -10, -10), 1);

    // Add event listeners for mouse movement
    this.isDragging = false;
    this.previousMousePosition = {
      x: 0,
      y: 0,
    };

    // Mouse events
    document.addEventListener("mousemove", this.onMouseMove.bind(this));
    document.addEventListener("mousedown", this.onMouseDown.bind(this));
    document.addEventListener("mouseup", this.onMouseUp.bind(this));
    document.addEventListener('wheel', this.handleMouseWheel.bind(this));
    this.animate = this.animate.bind(this);
    this.animate();

    // Initial call to fetch data for ROS
    this.fetchData();
  }

  loadSTL(loader, filePath) {
    return new Promise((resolve, reject) => {
        loader.load(filePath, resolve, undefined, reject);
    });
}
  createBox() {
    // Sphere for joint representation
    const radius = 3;
    const widthSegments = 32;
    const heightSegments = 32;
    const geometry = new THREE.SphereGeometry(
      radius,
      widthSegments,
      heightSegments
    );
    const material = new THREE.MeshStandardMaterial({
      color: 0xffffff,
      metalness: 1,
    });

    // Define object positions
    const positions = [
      { x: 50, y: -40, z: -73 },
      { x: 50, y: 40, z: -73 },
      { x: -50, y: -40, z: -73 },
      { x: -50, y: 40, z: -73 },
      { x: 50, y: -40, z: this.frame_Z },
      { x: 50, y: 40, z: this.frame_Z },
      { x: -50, y: -40, z: this.frame_Z },
      { x: -50, y: 40, z: this.frame_Z },
    ];

    // Array to store created meshes
    const boxes = [];

    // Create meshes and add them to the scene
    for (const position of positions) {
      const box = new THREE.Mesh(geometry, material);
      box.position.set(position.x, position.y, position.z);
      this.scene.add(box);
      boxes.push(box);
    }

    // Animate joints between meshes
    this.animateBone(this.p1, boxes[0], boxes[1]);
    this.animateBone(this.p2, boxes[0], boxes[2]);
    this.animateBone(this.p3, boxes[1], boxes[3]);
    this.animateBone(this.p4, boxes[3], boxes[2]);

    this.animateBone(this.p5, boxes[0], boxes[4]);
    this.animateBone(this.p6, boxes[1], boxes[5]);
    this.animateBone(this.p7, boxes[2], boxes[6]);
    this.animateBone(this.p8, boxes[3], boxes[7]);

    this.animateBone(this.p9, boxes[5], boxes[4]);
    this.animateBone(this.p10, boxes[6], boxes[4]);
    this.animateBone(this.p11, boxes[6], boxes[7]);
    this.animateBone(this.p12, boxes[7], boxes[5]);
  }

  // Function to handle mouse wheel event
  handleMouseWheel(event) {
    // Adjust camera's Y position based on the wheel delta
    this.camera.position.x += event.deltaY * 0.1; // Adjust the factor as needed
    // Ensure camera does not go below ground level or above certain limit
    // For example:
    // camera.position.y = Math.max(camera.position.y, minHeight);
    // camera.position.y = Math.min(camera.position.y, maxHeight);

    // Render the scene
  }

  onMouseMove(event) {
    if (this.isDragging) {
      const deltaMove = {
        x: event.offsetX - this.previousMousePosition.x,
        y: event.offsetY - this.previousMousePosition.y,
      };

      if (event.buttons === 1 && event.shiftKey) {
        // Left button pressed with Shift key
        // Adjust camera rotation based on mouse movement
        this.camera.rotation.y += deltaMove.x * 0.01;
        this.camera.rotation.x += deltaMove.y * 0.01;

      } else if (event.buttons === 1) {
        // Left button pressed
        // Adjust camera position based on mouse movement
        this.camera.position.z += deltaMove.y * 0.1;
        this.camera.position.y -= deltaMove.x * 0.1;

      } else if (event.buttons === 4) {
        // Middle button pressed
        // Adjust camera zoom based on mouse movement
        this.camera.position.x += deltaMove.y * 0.1;
        this.camera.position.y += deltaMove.y * 0.1;
        this.camera.position.z += deltaMove.z * 0.1;
      }

      this.previousMousePosition = {
        x: event.offsetX,
        y: event.offsetY,
      };
    }
  }

  onMouseDown(event) {
    this.isDragging = true;
    this.previousMousePosition = {
      x: event.offsetX,
      y: event.offsetY,
    };
  }

  onMouseUp(event) {
    this.isDragging = false;
  }

  createCylinder(joint1, joint2) {
    // Calculate the midpoint position between joint1 and joint2
    const centerX = (joint1.position.x + joint2.position.x) / 2;
    const centerY = (joint1.position.y + joint2.position.y) / 2;
    const centerZ = (joint1.position.z + joint2.position.z) / 2;

    // Calculate the height of the cylinder
    const distance = joint1.position.distanceTo(joint2.position);

    // Define dimensions for the rectangular prism
    const width = 2;
    const height = distance;
    const depth = 1; // Using the distance between joints as the depth

    // Create box geometry
    const geometry = new THREE.BoxGeometry(width, height, depth);

    // Create material
    const material = new THREE.MeshStandardMaterial({
      color: 0xffffff,
      metalness: 1,
    });

    // Create the cylinder
    const cylinder = new THREE.Mesh(geometry, material);

    // Calculate the direction from joint2 towards joint1
    const direction = new THREE.Vector3()
      .subVectors(joint1.position, joint2.position)
      .normalize();

    // Calculate cylinder position
    cylinder.position.set(centerX, centerY, centerZ);

    // Orient the cylinder
    cylinder.quaternion.setFromUnitVectors(
      new THREE.Vector3(0, 1, 0),
      direction
    );

    // Changes the joint orientation
    joint1.rotation.y = Math.atan2(cylinder.position.z - joint1.position.z, cylinder.position.x - joint1.position.x) + Math.PI/2;

    //joint1.rotation.z = Math.atan2(joint2.position.y - joint1.position.y, joint2.position.x - joint1.position.x);
    //joint1.rotation.x = Math.atan2(joint2.position.y - joint1.position.y, joint2.position.z - joint1.position.z) - 2*Math.PI;

    return cylinder;
  }

  addDirectionalLight(scene, position, intensity) {
    const light = new THREE.DirectionalLight(0xffffff, intensity);

    // Set light position
    light.position.copy(position);

    // Enable shadow casting
    light.castShadow = true;
    scene.add(light);
  }

  animateBone(con, joint1, joint2) {
    // Delete the previous cylinder
    if (con) {
      this.scene.remove(con);
    }

    // Adds to the scene the new cylinder
    con = this.createCylinder(joint1, joint2);
    this.scene.add(con);

    return con;
  }


  animate(){
    requestAnimationFrame(this.animate);
    this.renderer.render(this.scene, this.camera);
  }

  fullAnimation(){

    // Cables
    this.cableR = this.animateBone(
      this.cableR,
      this.cableSupportR,
      this.right_hip
    );

    this.cableL = this.animateBone(
      this.cableL,
      this.cableSupportL,
      this.left_hip
    );

    // Animates the connections between joints
    this.hip2KneeR = this.animateBone(
      this.hip2KneeR,
      this.right_hip,
      this.right_knee
    );
    this.hip2KneeL = this.animateBone(
      this.hip2KneeL,
      this.left_hip,
      this.left_knee
    );

    this.knee2AnkleR = this.animateBone(
      this.knee2AnkleR,
      this.right_knee,
      this.right_ankle
    );
    this.knee2AnkleL = this.animateBone(
      this.knee2AnkleL,
      this.left_knee,
      this.left_ankle
    );

    this.ankle2HeelR = this.animateBone(
      this.ankle2HeelR,
      this.right_ankle,
      this.right_heel
    );
    this.ankle2HeelL = this.animateBone(
      this.ankle2HeelL,
      this.left_ankle,
      this.left_heel
    );

    this.heel2ToeR = this.animateBone(
      this.heel2ToeR,
      this.right_heel,
      this.right_toe
    );
    this.heel2ToeL = this.animateBone(
      this.heel2ToeL,
      this.left_heel,
      this.left_toe
    );

    this.toe2AnkleR = this.animateBone(
      this.toe2AnkleR,
      this.right_toe,
      this.right_ankle
    );
    this.toe2AnkleL = this.animateBone(
      this.toe2AnkleL,
      this.left_toe,
      this.left_ankle
    );
  }

  //** Fetching data functions **/
  // Function to update the position of each joint
  updateData(data) {
    // Sets each articulation coord
    const joints = ["pelvis", "hip", "knee", "ankle", "heel", "toe"];
    const sides = ["left", "right"];

    for (const side of sides) {
      for (const joint of joints) {
        for (const axis of ["x", "y", "z"]) {
          const propertyName = `${side}_${joint}_${axis}`;
          const position = data[propertyName];
          
          // Check if this[`${side}_${joint}`] is defined
          if (this[`${side}_${joint}`]) {
            this[`${side}_${joint}`].position[axis] = position;
          } else {
            console.error(`Object ${side}_${joint} is undefined.`);
          }
        }
      }
    }
    this.fullAnimation()
  }

  // Function to continuously fetch data from the flask server
  fetchData() {
    // Fetch data
    fetch("/get_data")
      .then((response) => response.json())
      .then((data) => {
        this.updateData(data);
      })
      .catch((error) => console.error("Error:", error));

    // Timeout to update position
    setTimeout(() => this.fetchData(), 100); // Milliseconds
  }
}

// Starts all the program
const threeDScene = new ThreeDScene();
