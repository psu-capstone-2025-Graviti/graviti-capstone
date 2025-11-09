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
            lightProbe: Texture { source: "qrc:/Bodies/content/images/HubbleDeepField.jpg"}
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
                clipFar: 1000000000000000000000000
                clipNear: 0.01
            }


            //Trajectory Spheres
            Node {
                id: trajectorySphereContainer
                property var sphereDataList: []
                Repeater3D {
                    model: trajectorySphereContainer.sphereDataList
                    Model {
                        source: "#Sphere"
                        x: modelData.position.x
                        y: modelData.position.y
                        z: modelData.position.z

                        scale: Qt.vector3d(modelData.scale.x, modelData.scale.y, modelData.scale.z)
                        materials: [
                            PrincipledMaterial {
                                baseColor: modelData.materialColor
                                opacity: modelData.opacity
                            }
                        ]
                    }
                }

                function createTrajectorySpheres() {
                    var newSphereDataList = []
                    var count = trajectoryRenderer.count
                    for (var i = 0; i < count; i++) {
                        var sphereData = trajectoryRenderer.trajectorySpheres[i]
                        if (sphereData) {
                            newSphereDataList.push(sphereData)
                        }
                    }
                    sphereDataList = newSphereDataList
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
                property var entityDataList: []
                Repeater3D {
                    //id: entitySphereComponent
                    model : entitySphereContainer.entityDataList
                    Model {
                        source: "#Sphere"
                        x: modelData.position.x
                        y: modelData.position.y
                        z: modelData.position.z

                        // Scale from trajectoryRenderer
                        scale: Qt.vector3d(modelData.scale.x, modelData.scale.y, modelData.scale.z)

                        // Material based on entity type or index
                        materials: [
                            PrincipledMaterial {
                                baseColor: modelData.materialColor
                                baseColorMap: modelData.texturePath && modelData.texturePath !== "" ? textureMap : null
                                roughness: 0.5
                                opacity: modelData.opacity

                                property Texture textureMap: Texture {
                                    source: modelData.texturePath || ""
                                }
                            }
                        ]
                    }
                }

                Component.onCompleted: {
                    createEntitySpheres()
                }

                function createEntitySpheres() {
                    var newEntityDataList = []
                    var count = trajectoryRenderer.entitySphereCount

                    for (var i = 0; i < count; i++) {
                        var sphereData = trajectoryRenderer.entitySpheres[i]
                        if(sphereData){
                            newEntityDataList.push(sphereData)
                        }
                    }
                    entityDataList = newEntityDataList
                }

                Connections {
                    target: trajectoryRenderer
                    function onEntitySpheresChanged() {
                        entitySphereContainer.createEntitySpheres()
                        root.updateCameraPosition()
                    }
                }
            }

            //Flat EntitiesSpheres
            Node {
                id: flatEntitiesContainer
                property var entityDataList: []
                Repeater3D {
                    //id: entitySphereComponent
                    model : trajectoryRenderer.flatEntities
                    Model {
                        source: "#Rectangle"
                        x: modelData.position.x
                        y: modelData.position.y
                        z: modelData.position.z

                        eulerRotation.x: 0
                        eulerRotation.y: 0
                        eulerRotation.z: 0


                        // Scale from trajectoryRenderer
                        scale: Qt.vector3d(modelData.scale.x, modelData.scale.y, modelData.scale.z)

                        // Material with texture, transparency and double-sided rendering (unlit via PrincipledMaterial.NoLighting)
                        materials: [
                            PrincipledMaterial {
                                id: flatMat
                                lighting: PrincipledMaterial.NoLighting
                                baseColorMap: modelData.texturePath && modelData.texturePath !== "" ? textureMap : null
                                alphaMode: PrincipledMaterial.Blend
                                cullMode: Material.NoCulling
                                opacity: modelData.opacity


                                property Texture textureMap: Texture {
                                    source: modelData.texturePath || ""
                                }
                            }
                        ]
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
    property real moveSpeed: 10.0
    
    property bool isMousePressed: false
    property point lastMousePos: Qt.point(0, 0)

    property real cameraOffsetX: 0
    property real cameraOffsetY: 0
    property real cameraOffsetZ: 0



    //KEYBOARD INPUT FOR CAMERA
    //This section of code handles the capture and processing of keyboard inputs
    //Currently, this code has a fixed move speed for the camera, and only one key is processed
    //at a time
    Keys.onPressed: (event) => {
        //Camera move speed in "units"

        //Base camera zoom is 350, we want values below this to slow move speed and values above to increase
        var zoomscale = cameraDistance / 350
        moveSpeed = zoomscale * 10

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
        
        onPressed: function(mouse) {
            isMousePressed = true
            lastMousePos = Qt.point(mouse.x, mouse.y)
        }
        
        onReleased: {
            isMousePressed = false
        }
        
        onPositionChanged: function(mouse) {
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
        
        onWheel: function(wheel) {
            var zoomFactor = wheel.angleDelta.y > 0 ? (1 - zoomSpeed) : (1 + zoomSpeed)
            cameraDistance *= zoomFactor
            cameraDistance = Math.max(1, Math.min(1e24, cameraDistance))
            
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

        var target = Qt.vector3d(0 + dx, 0 + dy, 0 + dz)

        var emptyPos = Qt.vector3d(0, 0, 0)

        if(trajectoryRenderer.camPosition != emptyPos)
        {
            target = trajectoryRenderer.camPosition
        }

        sceneCamera.position = Qt.vector3d(target.x + x, target.y + y, target.z + z)
        sceneCamera.lookAt(target)

    }

    // Connect to trajectoryRenderer changes to cameraposition
    Connections {
        target: trajectoryRenderer
        function onCamPositionChanged() {
            updateCameraPosition()
        }
    }
}
