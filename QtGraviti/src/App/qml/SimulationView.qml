import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick3D
import GravitiLib 1.0

Item {
    id: root
    width: 1280
    height: 720
    focus: true
    antialiasing: true
    

    
    Item {
        id: __materialLibrary__

        PrincipledMaterial {
            id: principledMaterial
            baseColor: "#717171"
            objectName: "New Material"
        }

        SpecularGlossyMaterial {
            id: specularGlossy
            specularColor: "#000000"
            objectName: "New Material"
            albedoColor: "#454eee"
        }
    }

    View3D {
        id: view3D
        anchors.fill: parent
        antialiasing: true
        environment: sceneEnvironment1
        camera: sceneCamera
        SceneEnvironment {
            id: sceneEnvironment1
            antialiasingQuality: SceneEnvironment.High
            antialiasingMode: SceneEnvironment.MSAA
            backgroundMode: SceneEnvironment.SkyBox
            lightProbe: Texture { source: "qrc:/content/images/HubbleDeepField.jpg"}
        }

        Node {
            id: scene
            DirectionalLight {
                id: directionalLight
                eulerRotation.y: 46
                eulerRotation.x: -33
                eulerRotation.z: -5
            }

            PerspectiveCamera {
                id: sceneCamera
                z: 350
            }


            //Trajectory Spheres
            Node {
                id: trajectorySphereContainer
                Component {
                    id: trajectorySphereComponent
                    Model {
                        id: trajectorySphere
                        objectName: "trajectorySphere"
                        source: "#Sphere"

                        property int sphereIndex: 0
                        property var sphereData: null

                        x: sphereData.position.x
                        y: sphereData.position.y
                        z: sphereData.position.z

                        // Scale from trajectoryRenderer
                        scale: Qt.vector3d(sphereData.scale.x, sphereData.scale.y, sphereData.scale.z)

                        // Material based on entity type or index
                        materials: [
                            PrincipledMaterial {
                                baseColor: sphereData.materialColor
                            }
                        ]

                        Connections {
                            target: trajectoryRenderer
                            function onTrajectorySpheresChanged() {
                                if (trajectoryRenderer && sphereIndex < trajectoryRenderer.count) {
                                    dynamicTrajectorySphere.sphereData = trajectoryRenderer.trajectorySpheres[sphereIndex]
                                }
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    createTrajectorySpheres()
                }

                function createTrajectorySpheres() {
                    var count = trajectoryRenderer.count

                    for (var i = 0; i < count; i++) {
                        var sphereData = trajectoryRenderer.trajectorySpheres[i]
                        if (sphereData) {
                            var sphere = trajectorySphereComponent.createObject(trajectorySphereContainer, {
                                "sphereIndex": i,
                                "sphereData": sphereData
                            })
                        }
                    }
                }
                // Connect to trajectoryRenderer changes to recreate spheres
                Connections {
                    target: trajectoryRenderer
                    function onTrajectorySpheresChanged() {
                        trajectorySphereContainer.createTrajectorySpheres()
                    }
                }
            }

            //Entity Spheres
            Node {
                id: entitySphereContainer
                Component {
                    id: entitySphereComponent
                    Model {
                        id: entitySphere
                        objectName: "entitySphere"
                        source: "#Sphere"

                        property int sphereIndex: 0
                        property var sphereData: null

                        x: sphereData.position.x
                        y: sphereData.position.y
                        z: sphereData.position.z

                        // Scale from trajectoryRenderer
                        scale: Qt.vector3d(sphereData.scale.x, sphereData.scale.y, sphereData.scale.z)

                        // Material based on entity type or index
                        materials: [
                            PrincipledMaterial {
                                baseColor: sphereData.materialColor
                                baseColorMap: sphereData.texturePath && sphereData.texturePath !== "" ? textureMap : null
                                roughness: 0.5

                                property Texture textureMap: Texture {
                                    source: sphereData.texturePath || ""
                                }

                                Component.onCompleted: {
                                    console.log("Entity sphere created with texturePath:", sphereData.texturePath)
                                }
                            }
                        ]

                        Connections {
                            target: trajectoryRenderer
                            function onEntitySpheresChanged() {
                                if (trajectoryRenderer && sphereIndex < trajectoryRenderer.entitySphereCount) {
                                    entitySphere.sphereData = trajectoryRenderer.entitySpheres[sphereIndex]
                                }
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    createEntitySpheres()
                }

                function createEntitySpheres() {
                    var count = trajectoryRenderer.entitySphereCount

                    for (var i = 0; i < count; i++) {
                        var sphereData = trajectoryRenderer.entitySpheres[i]
                        if (sphereData) {
                            var sphere = entitySphereComponent.createObject(entitySphereContainer, {
                                "sphereIndex": i,
                                "sphereData": sphereData
                            })
                        }
                    }
                }
                Connections {
                    target: trajectoryRenderer
                    function onEntitySpheresChanged() {
                        entitySphereContainer.createEntitySpheres()
                    }
                }
            }

        }
    }

    property real cameraDistance: 350
    property real cameraRotationX: 0
    property real cameraRotationY: 0
    property real zoomSpeed: 0.1
    property real rotationSpeed: 0.5
    property real panSpeed: 0.01
    
    property bool isMousePressed: false
    property point lastMousePos: Qt.point(0, 0)
    property bool isPanning: false

    property real cameraOffsetX: 0
    property real cameraOffsetY: 0
    property real cameraOffsetZ: 0



    //KEYBOARD INPUT FOR CAMERA
    //This section of code handles the capture and processing of keyboard inputs
    //Currently, this code has a fixed move speed for the camera, and only one key is processed
    //at a time
    Keys.onPressed: (event) => {
        //Camera move speed in "units"
        //TODO - make scale with camera zoom?
        var moveSpeed = 10

        switch(event.key){
            case Qt.Key_W:
                // Forwards
                cameraOffsetX -= moveSpeed * Math.sin(cameraRotationY * Math.PI / 180)
                cameraOffsetZ -= moveSpeed * Math.cos(cameraRotationY * Math.PI / 180)
                event.accepted = true; // Stop event propagation
                break;
            case Qt.Key_S:
                // Backwards
                cameraOffsetX += moveSpeed * Math.sin(cameraRotationY * Math.PI / 180)
                cameraOffsetZ += moveSpeed * Math.cos(cameraRotationY * Math.PI / 180)
                event.accepted = true; // Stop event propagation
                break;
            case Qt.Key_A:
                // Right
                cameraOffsetX -= moveSpeed * Math.cos(cameraRotationY * Math.PI / 180)
                cameraOffsetZ += moveSpeed * Math.sin(cameraRotationY * Math.PI / 180)
                event.accepted = true; // Stop event propagation
                break;
            case Qt.Key_D:
                // Left
                cameraOffsetX += moveSpeed * Math.cos(cameraRotationY * Math.PI / 180)
                cameraOffsetZ -= moveSpeed * Math.sin(cameraRotationY * Math.PI / 180)
                event.accepted = true; // Stop event propagation
                break;

            case Qt.Key_Q:
                // Up
                cameraOffsetY += moveSpeed
                event.accepted = true; // Stop event propagation
                break;
            case Qt.Key_E:
                // Down
                cameraOffsetY -= moveSpeed
                event.accepted = true; // Stop event propagation
                break;
        }

        updateCameraPosition()
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        
        onPressed: {
            isMousePressed = true
            lastMousePos = Qt.point(mouse.x, mouse.y)
        }
        
        onReleased: {
            isMousePressed = false
        }
        
        onPositionChanged: {
            if (!isMousePressed) return
            
            var deltaX = mouse.x - lastMousePos.x
            var deltaY = mouse.y - lastMousePos.y
            
            cameraRotationY += deltaX * rotationSpeed
            cameraRotationX += deltaY * rotationSpeed
                
            // Calmp to prevent flipping
            cameraRotationX = Math.max(-89, Math.min(89, cameraRotationX))

            updateCameraPosition()
            
            
            lastMousePos = Qt.point(mouse.x, mouse.y)
        }
        
        onWheel: {
            var zoomFactor = wheel.angleDelta.y > 0 ? (1 - zoomSpeed) : (1 + zoomSpeed)
            cameraDistance *= zoomFactor
            cameraDistance = Math.max(10, Math.min(1000, cameraDistance))
            
            updateCameraPosition()
        }
    }


    
    function updateCameraPosition() {
        var radiansX = cameraRotationX * Math.PI / 180
        var radiansY = cameraRotationY * Math.PI / 180
        
        var x = cameraDistance * Math.cos(radiansX) * Math.sin(radiansY)
        var y = cameraDistance * Math.sin(radiansX)
        var z = cameraDistance * Math.cos(radiansX) * Math.cos(radiansY)


        var dx = cameraOffsetX
        var dy = cameraOffsetY
        var dz = cameraOffsetZ

        sceneCamera.position = Qt.vector3d(x + dx, y + dy, z + dz)
        sceneCamera.lookAt(Qt.vector3d(0 + dx, 0 + dy, 0 + dz))
    }
}

/*##^##
Designer {
    D{i:0;matPrevEnvDoc:"SkyBox";matPrevEnvValueDoc:"preview_studio";matPrevModelDoc:"#Sphere"}
D{i:6;cameraSpeed3d:25;cameraSpeed3dMultiplier:1}
}
##^##*/
