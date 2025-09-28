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
        
        sceneCamera.position = Qt.vector3d(x, y, z)
        sceneCamera.lookAt(Qt.vector3d(0, 0, 0))
    }
}

/*##^##
Designer {
    D{i:0;matPrevEnvDoc:"SkyBox";matPrevEnvValueDoc:"preview_studio";matPrevModelDoc:"#Sphere"}
D{i:6;cameraSpeed3d:25;cameraSpeed3dMultiplier:1}
}
##^##*/
