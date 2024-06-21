# Nimble

<!--Añadir imagen del logo-->

<div align="center">
<img width=100px src="https://img.shields.io/badge/status-in%20process-orange" alt="In process"></a>

<!-- <img width=100px src="https://img.shields.io/badge/license-Apache-green" alt="explode"></a> -->
</div>

## Índice

1. [Instalación del paquete](#instalación-del-paquete)

2. [Anotaciones](#anotaciones)

3. [Nodos](#nodos)
   * [HMI](https://github.com/jcarballeira/nimble/blob/HMI/docs/hmi.md)
   * [Kinematic Model](https://github.com/jcarballeira/nimble/blob/HMI/docs/kinematic_model.md)
   * [Cables robot](https://github.com/jcarballeira/nimble/blob/HMI/docs/cables_robot.md)
   * [Control MSD](https://github.com/jcarballeira/nimble/blob/HMI/docs/Control_MSD.md)
   * [Control ZMP](https://github.com/jcarballeira/nimble/blob/HMI/docs/Control_ZMP.md)
   * [Frame](https://github.com/jcarballeira/nimble/blob/HMI/docs/frame.md)
   * [Gaming](https://github.com/jcarballeira/nimble/blob/HMI/docs/gaming.md)
   * [States Machine](https://github.com/jcarballeira/nimble/blob/HMI/docs/states_machine.md)
   * [Simulink Node](https://github.com/jcarballeira/nimble/blob/HMI/docs/simulink_node.md)

4. [Tipos de Mensaje Utilizados](#tipos-de-mensaje-utilizados)

5. [Equipo](#equipo)

---
---

## Instalación del paquete

0. **Requisitos sistema:**

* Ubuntu 22.04
* Instalación desktop de ROS2 Humble

1. **Crear una carpeta workspace** (p.e. nimble_ws o ros_ws) y subcarpeta nimble_ws/src. En ella copiaremos los archivos descargandolos directamente o ejecutando:

```bash
git clone https://github.com/jcarballeira/nimble.git
```

3. **Compilar:** Situarse en "cd nimble_ws" y ejecutar "colcon build". Rezar a vuestra divinidad favorita.

4. **Cargar la instalación** con "source install/setup.bash"

* Este paso hay que realizarlo desde nimble_ws al abrir un nuevo terminal. Para evitarlo, abrir un terminal nuevo, ejecutar "gedit .bashrc"
          Se abrirá un archivo en el que debemos copiar al final la siguiente instrucción:

   ```bash
   source ~/nimble_ws/install/setup.bash
   ```

   De esta forma se ejecuta automáticamente al abrir un nuevo terminal. De todas formas, al compilar algo nuevo habrá que reiniciar el terminal para que se
          actualice ese archivo.

5. **Ejecución del nodo** deseado "ros2 run nimble your_node"

6. En el caso del mainPC existe un fichero launch para ejecutar todos los nodos en una única instrucción. Ejecutar "ros2 launch nimble nimble_launch.py".

---
---

## Anotaciones

* Existen 2 paquetes diferentes e imprescindibles:

  * nimble_interfaces: Contiene el servicio para la generacion de trayectorias ideales (NN) y los mensajes creados para almacenar información  de forma concreta (para el resto se usan msgs predefinidos en ROS2). Al final del documento hay una lista de los tipos de mensaje creados y los distintos campos que contienen

  * nimble: Incluye todos los nodos a ejecutar tanto en el mainPC como el los diferentes micros o raspberrys.

Cada nodo está suscrito a todos los topics indicados y preparado para publicar lo necesario (o eso creo).

Tiene las funciones asociadas a la recepción de cada mensaje (callbacks). En esos callbacks se almacena cada mensaje en una estructura de datos para procesarlos según el objetivo de cada nodo. La estructura está pensada para almacenar los mensajes según lleguen y poder hacer el procesamiento necesario según el nodo. De esta manera se puede comprobar si ya llegó toda la información, subdividirlo en varias funciones de procesamiento si solo se necesitan 2 mensajes, directamente en el callback si solo hace falta un mensaje...según cada nodo.

Además cada nodo tiene un timer (de perido ajustable, default:1000ms) que ejecuta un callback por si se quiere publicar o ejecutar algo periódicamente. Típicamente los sensores se publican cada cierto tiempo, habrá casos en los que solo se quiera publicar si ocurre algo concreto, por ejemplo para enviar un set point. En caso de no usarlo eliminadlo para evitar que entre innecesariamente.

Ahora que cada cual haga sus chapuzas correspondientes en cada nodo. Salud y república. Deslicen hacia abajo para más info interesante, suscribanse y denle a like.

---
---

## TIPOS DE MENSAJE UTILIZADOS

### Mensajes y servicios custom y los campos que contienen

#### nimble_interfaces/msg/Measurements

- `std_msgs/Header header`
* `float64 height`
* `float64 femur`
* `float64 tibia`
* `float64 height_ankle`
* `float64 distance_to_heel`
* `float64 distance_to_toe`
* `float64 depth_pelvis`
* `float64 width_pelvis`

#### nimble_interfaces/msg/TherapyRequirements

- `std_msgs/Header header`
* `float64 speed`
* `float64 step_length`
* `float64 step_height`
* `int32 min_assist`
* `int32 max_assist`

#### nimble_interfaces/msg/FrameState

- `std_msgs/Header header`
* `float64 optic_sensor`
* `float64 velocity`
* `float64 position`

#### nimble_interfaces/msg/CartesianTrajectory

- `std_msgs/Header header`
* `geometry_msgs/Point[] malleolus` (vector de vectores [x,y,z])
  * `float64 x`
  * `float64 y`
  * `float64 z`
* `geometry_msgs/Point[] pelvis` (vector de vectores [x,y,z])
  * `float64 x`
  * `float64 y`
  * `float64 z`

#### nimble_interfaces/srv/TrajGeneratorService (a partir de medidas y necesidades de paso genera la trayectoria ideal articulaciones)

- Request 1: `measurements` (nimble_interfaces/msg/Measurements)
* Request 2: `therapy_requirements` (nimble_interfaces/msg/TherapyRequirements)
* Response: `joints_target` (trajectory_msgs/msg/JointTrajectory)

#### nimble_interfaces/src/Cartesian full trajectory_

- `std_msgs/Header header`
* `geometry_msgs/Point[] left_pelvis`
* `geometry_msgs/Point[] right_pelvis`
* `geometry_msgs/Point[] base_pelvis`
* `geometry_msgs/Point[] left_hip`
* `geometry_msgs/Point[] right_hip`
* `geometry_msgs/Point[] left_knee`
* `geometry_msgs/Point[] right_knee`
* `geometry_msgs/Point[] left_ankle`
* `geometry_msgs/Point[] right_ankle`
* `geometry_msgs/Point[] left_heel`
* `geometry_msgs/Point[] right_heel`
* `geometry_msgs/Point[] left_toe`
* `geometry_msgs/Point[] right_toe`

---
---

## Mensajes utilizados de los predefinidos en ROS

### sensor_msgs/msg/JointState

- `std_msgs/Header header`
* `string[] name` (seguiremos este orden ["hipR", "kneeR", "ankleR","hipL", "kneeL", "ankleL"])
* `float64[] position` (vector de vectores [pos_hipR, pos_kneeR, pos_ankleR, pos_hipL, pos_kneeL, pos_ankleL])
* `float64[] velocity` (análogo al anterior con velocidades)
* `float64[] effort` (análogo al anterior con velocidades)

### trajectory_msgs/msg/JointTrajectory

- `std_msgs/Header header`
* `string[] joint_names` (seguiremos este orden ["hipR", "kneeR", "ankleR","hipL", "kneeL", "ankleL"])
* `JointTrajectoryPoint[] points`
  * `float64[] positions`
  * `float64[] velocities`
  * `float64[] accelerations`
  * `float64[] effort`
  * `duration time_from_start`

### std_msgs/Int32MultiArray

- `MultiArrayLayout layout`
* `int32[] data`

### std_msgs/Float32MultiArray

- `MultiArrayLayout layout`
* `float32[] data`

### std_msgs/Int16

- `int16 data`

 ---
 ---

## Equipo

* [Juan Carballeira](https://github.com/jcarballeira)
* [Jaime Ramos](https://github.com/jaimeramosr)
* [Adrián Madinabeitia](https://github.com/Adrimapo)
  
