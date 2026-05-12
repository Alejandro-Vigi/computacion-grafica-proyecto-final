# Entorno Virtual Interactivo: Área de cajas de la planta baja del edificio A de la Facultad de Ingeniería (FI-UNAM).

![Logo de la Facultad de Ingeniería UNAM](https://www.unamenlinea.unam.mx/sites/default/files/recursos/img/facultad-de-ingenieria.jpg)
<br>
*Figura 1: Proyecto desarrollado para la materia de Computación Gráfica e Interacción Humano-Computadora 2026-2.*

## 📌 Descripción del Proyecto
Este proyecto consiste en un entorno virtual interactivo tridimensional que reproduce la planta baja del **Edificio A de la Facultad de Ingeniería (UNAM).**

## 🏛️ Escenario y Contexto
El entorno virtual se sitúa en la planta baja del Edificio A (Principal) de la Facultad de Ingeniería de la UNAM, específicamente en el área de cajas. Este espacio fue seleccionado por ser uno de los puntos designado para eventos de reclutamiento y ferias del empleo. El modelo incluye una representación fiel de los elementos arquitectónicos del sitio, como sus columnas circulares de concreto, el piso cuadriculado característico y la zona administrativa de ventanillas con marcos amarillos.

## 🛠️ Stack Tecnológico
Para garantizar un alto rendimiento y control total sobre el renderizado, se utilizó:

**Lenguaje:** C++.  
**Gráficos:** OpenGL.  
**Librerías:**  
* `GLEW` & `GLFW` para gestión de contextos y ventanas.  
* `GLM` para operaciones matemáticas y transformaciones.  
* `Assimp` para la importación de modelos complejos (.fbx, .obj).  
* `SOIL2` & `stb_image` para el manejo de texturas.

## 🎮 Funcionalidades e Interacción
El sistema integra múltiples elementos de **Interacción Humano-Computadora** para ofrecer una experiencia inmersiva:

* **Navegación:** Control de cámara libre y fluida mediante teclado (`W, A, S, D`) y mouse.
* **Ciclo Día/Noche:** Cambio dinámico de iluminación (luces direccionales, fijas y spots) y Skybox presionando la tecla `P`.
* **Sistema de Stands:** Detección de clics mediante Ray Casting sobre el piso para activar la construcción animada de stands específicos a través de Keyframes.
* **Animaciones Complejas:**  
    * **Robot de bienvenida:** Secuencia de saludo y señalización (tecla `G`).  
    * **Flujo de visitantes:** Simulación de múltiples personas caminando por el lobby (tecla `H`).  
    * **Interacción de lanzamiento:** Una persona realizando un tiro de baloncesto hacia un aro, con física de trayectoria animada (tecla `L`).  
    * **Estudiante en el lobby:** Animación de una mujer caminando y hablando por teléfono (tecla `I`).  
    * **Entorno interactivo:** NPCs en conversación y revisión de material informativo.

---
### 🖼️ Galería de Referencia

*Figura 2: Vista general del entorno virtual renderizado en tiempo real.*

*Figura 3: Plano esquemático de la planta baja - Área de cajas FI-UNAM.*

---

## 📈 Gestión del Proyecto
El desarrollo se llevó a cabo bajo la metodología ágil **Scrum**, organizada en sprints semanales para la construcción incremental del entorno:

1. **Modelado:** Espacio físico del lobby (columnas, ventanillas, accesos) diseñado en **Blender**.
2. **Animación:** Implementación de técnicas de interpolación y jerarquía de huesos para personajes FBX.
3. **Optimización:** Uso de luces Spot y Point lights para simular la iluminación real del edificio.

## 👥 Integrantes - Equipo 10
**Integrante 1** - 320301159  
**Integrante 2** - 319159709  
**Integrante 3** - 319033515  

**Profesor:** Arturo Pérez de la Cruz  
**Fecha de entrega:** 13 de mayo de 2026

---
*Este proyecto es de carácter académico para la Universidad Nacional Autónoma de México.*
