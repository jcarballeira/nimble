# HMI

<div align="center">
    <img width=100px src="https://img.shields.io/badge/lenguage-%20python-blue" alt="Python">
    <img width=100px src="https://img.shields.io/badge/status-in%20process-orange" alt="In process">
</div>

## Objetivo

Publicar periódicamente las medidas del paciente y los requisitos de la terapia. Se puede sustituir por publicar esos topics directamente en la terminal o creando una interfaz real. Los parámetros pueden modificarse por terminal (en caliente) inicialmente se cargan en el fichero launch/nimble_launch.py.

---

## Topics
* **Suscripciones:**

  * `/cartesian_trajectory` (nimble_interfaces/msg/CartesianTrajectory)
    - **Origen:** kinematic model
    - Recrea la ruta que se le pasa al paciente.
  
  * `/cartesian_state` (nimble_interfaces/msg/CartesianFullTrajectory)
    - **Origen:** kinematic model 
    - Se utiliza para mostrar la posición acutal del exo.  
  
* **Publicaciones:**

  * `/measurements` (nimble_interfaces/msg/Measurements)
    - Medidas físicas del paciente.
  * `/therapy_requirements` (nimble_interfaces/msg/TherapyRequirements)
    - Niveles de asistencia, requisitos de altura y distancia de paso.

---

## Ejecución

1. Lanzar el launcher general:

```bash
ros2 launch nimble nimble_launch.py
```

2. Abrir navegador predeterminado y acceder a la dirección <http://127.0.0.1:5000>

## Estructura de directorios

```java
hmi_web/
├── flask_class.py: Es la clase que utilizará el nodo de ROS para comunicarse con la web.
├── package.json
├── package-lock.json
├── sender.py
├── static/
│   ├── main.js: Programa de JavaScript donde está todo el código de la animación 3D del exoesqueleto, además de la comunicación de la web con Flask.
│   ├── models
│   └── style.css: Estilo de la página.
└── templates/
    ├── index.html: Estructura web del HMI.
    ├── package.json
    └── package-lock.json
```


* **models:** Contiene los modelos 3D del exoesqueleto, cada articulación se asocia a su respectivo stl con la siguiente representación permitiendo no tener que llamar parte a parte:

```javascript
    var jointData = [
      { name: 'right_knee', orientation: new THREE.Euler(0, Math.PI, 0), 
      file: 'case'},

      { name: 'left_knee', orientation: new THREE.Euler(0, Math.PI, 0), 
      file: 'case' } 
      
      ...];
```
    Nota: Algunos .stl fueron modificados ya que sus centros no eran los mismos que el centro geométrico de la figura.


## Versiones del HMI

### Versión 1
<figure class="align-center" style="width:60%">
  <img src="https://github.com/jcarballeira/nimble/blob/HMI/docs/assets/images/hmiV1.png" alt="">
  <figcaption>V 1.0</figcaption>
</figure>

Primera versión con figuras geométricas como exoesqueleto.

### Versión 2
<figure class="align-center" style="width:60%">
  <img src="https://github.com/jcarballeira/nimble/blob/HMI/docs/assets/images/hmiV2.png" alt="">
  <figcaption>V 2.0</figcaption>
</figure>

Se añadieron los STLs para tener una vista más agradable del exoesqueleto.

<figure class="align-center" style="width:60%">
  <img src="https://github.com/jcarballeira/nimble/blob/HMI/docs/assets/images/hmiV2-1.png" alt="">
  <figcaption>V 2.1</figcaption>
</figure>

Exoesqueleto "Sombra" para mostrar el movimiento actual del exoesqueleto junto al movimiento ideal.

