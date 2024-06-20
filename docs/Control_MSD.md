# Control MSD

<div align="center">
    <img width=100px src="https://img.shields.io/badge/lenguage-%20c++-blue" alt="c++">
    <img width=100px src="https://img.shields.io/badge/status-in%20process-orange" alt="In process">
</div>

## Objetivo

Control MSD de las articulaciones del Exo H3


---

## Topics

**Suscribed:**

- `/joints_target` (nimble_interfaces/msg/JointsTrajectory)
  - **Origen:** states_machine
  - Objetivo actual de las articulaciones publicado a periodo constante (segun velocidad caminata), incluye fase y vector ángulos.

- `/joints_state` (sensor_msgs/msg/JointState)
  - **Origen:** Nodo_Simulink (H3)
  - Información actual de las articulaciones del H3

- `/assistLevel` (std_msgs/Int32MultiArray)
  - **Origen:** states_machine
  - Vector nivel de asistencia para cada articulación

- `/interactionTorque` (std_msgs/Float32MultiArray)
  - **Origen:** Nodo_Simulink (H3)
  - Torque de interacción medido por el H3, 6 elementos ["hipR", "kneeR", "ankleR","hipL", "kneeL", "ankleL"]

- `/FSR` (std_msgs/ByteMultiArray)
  - **Origen:** Nodo_Simulink (H3)
  - Contacto con el suelo, 4 elementos [heelR, toeR, heelL, toeL] 

**Publishes:**

- `/joints_SP` (sensor_msgs/msg/JointState)
  - Set points enviados al Exo (posición, velocidad o torque)
