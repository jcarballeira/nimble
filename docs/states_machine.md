# States machine

<div align="center">
    <img width=100px src="https://img.shields.io/badge/lenguage-%20c++-blue" alt="c++">
    <img width=100px src="https://img.shields.io/badge/status-in%20process-orange" alt="In process">
</div>

## Objetivo

Reclama el servicio de generacion de trayectorias ante cada modificacion de las medidas o requisitos de la terapia. Para esta trayectoria ideal ajusta el numero de puntos según velocidad de avance del paciente y el periodo que reclame el control MSD. Publica toda la trayectoria y punto a punto con el porcentaje de marcha asociado. Funciones por definir para la evaluacion de las trayectoria real vs ideal (cartesiana y articular?), el ajuste del nivel de asistencia en consecuencia.

---

## Topics

**Suscribed:**
- `/measurements` (nimble_interfaces/msg/Measurements)
  - **Origen:** hmi
  - Medidas físicas del paciente
 
- `/therapy_requirements` (nimble_interfaces/msg/TherapyRequirements)
  - **Origen:** hmi
  - objetivo dist y altura de paso, rangos de asistencia,velocidad...

- `/joints_state` (sensor_msgs/msg/JointState)
  - **Origen:** Nodo_Simulink (H3)
  - Información actual de las articulaciones del H3
 
- `/cartesian_state` (nimble_interfaces/msg/CartesianTrajectory)
  - **Origen:** kinematic_model
  - posiciones xyz para la trayectoria completa de un paso (pelvis, maleolo, toe, heel)
 
- `/cartesian_trajectory` (nimble_interfaces/msg/CartesianTrajectory)
  - **Origen:** kinematic_model
  - posiciones xyz acumuladas hasta el instante actual desde el inicio del paso (pelvis, maleolo, toe, heel) 

- `/cables_state` (sensor_msgs/msg/JointState)
  - **Origen:** cables_robot
  - Posición, esfuerzo, etc. de los cables (por definir)
 
- `/step_target` (nimble_interfaces/msg/TherapyRequirements)
  - **Origen:** kinematic model
  - altura y longitud de paso a ejecutar, consecuencia del calculo cinematico usando la trayectoria articular ideal
 
- `/interactionTorque` (std_msgs/Float32Multiarray)
  - **Origen:** Nodo_Simulink (H3)
  - torque de interacción medido por el H3, 6 elementos ["hipR", "kneeR", "ankleR","hipL", "kneeL", "ankleL"]
 
- `/FSR` (std_msgs/ByteMultiarray)
  - **Origen:** Nodo_Simulink (H3)
  - sensores de contacto con el suelo, 4 elementos [heelR, toeR, heelL, toeL] 



**Publishes:**

- `/joints_trajectory` (nimble_interfaces/msg/JointsTrajectory)
    - trayectoria articular ideal obtenida por servicio (NN: TrajGenerationService) e interpolada para ajustar al periodo que requiere el control MSD

- `/joints_target` (nimble_interfaces/msg/JointsTrajectory)
    - objetivo actual del control MSD (joints_trajectory punto por punto)
 
- `/assistLevel` (std_msgs/Int32Multiarray)
    - vector nivel de asistencia para cada articulacion ["hipR", "kneeR", "ankleR","hipL", "kneeL", "ankleL"]
 
- `/executionMode` (std_msgs/Int32)
    - modo de ejecucion unico (release, transparent...)
 
- `/controlMode` (std_msgs/Int32)
    - modo de control para cada articulacion ["hipR", "kneeR", "ankleR","hipL", "kneeL", "ankleL"]
