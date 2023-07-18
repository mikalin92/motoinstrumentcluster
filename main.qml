// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtLocation
import QtPositioning
import QtQuick.Window
Window {
    property string mycolor;
    property var arrpoints : [];
    property var ccoords: [];
    property var thisccoordsdata: [];

    property bool mapVisibleKey: true;
    property var motodata: [];
    property var thismotodata: [];

    property real angle: 0;
    property real rpmangle: 0;

    property real kmph: 0;
    property real rpm: 0;
    property int gear: 0;

    property real dotsize: 20;
    property var nowposition: QtPositioning.coordinate(65.011, 25.472);//Oulu
    property real heading: 0.0;

    id: mainWin1
    objectName: "mainWin1"
    //width: Qt.platform.os == "android" ? Screen.width : 1024
    //height: Qt.platform.os == "android" ? Screen.height : 512
    width: 1024
    height: 512
    visible: true
    title: map.center + " zoom " + map.zoomLevel.toFixed(3)
           + " min " + map.minimumZoomLevel + " max " + map.maximumZoomLevel

    onCcoordsChanged:   {
        thisccoordsdata[0]={"e": ccoords[0]    , "n": ccoords[1],"m":ccoords[2]}
        //rec0.requestPaint()

        nowposition=QtPositioning.coordinate(0+thisccoordsdata[0]["n"], 0+thisccoordsdata[0]["e"]);
        heading=(0+thisccoordsdata[0]["m"])*(180/Math.PI);
    }



    onMotodataChanged: {
        thismotodata[0]={"kmph": motodata[0]    , "rpm": motodata[1],"gear": motodata[2]};
        kmph=0+thismotodata[0]["kmph"];
        gear=0+thismotodata[0]["gear"]
        angle=(Math.PI/2)*kmph/200;
        rpmangle=(Math.PI/2)*(0+0+thismotodata[0]["rpm"])/4000;

        //arrpoints[0]={"x":kmph,"y":kmph };
        rec0.requestPaint()

    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }



    Canvas {

        id: rec0
        objectName: "rec0"
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.top: parent.top



        onPaint: {
            var context = getContext("2d")
            context.reset();
            arrpoints[0]={"x":rec0.width-rec0.width*(Math.cos(angle)),"y":rec0.height-rec0.height*(Math.sin(angle)) };
            context.beginPath();
            context.strokeStyle = Qt.rgba(0, 0, 0, 1)
            context.lineWidth = 1
            context.font= "70px sans-serif"

            var point = arrpoints[0]
            context.ellipse(0, 0, 2*width, 2*height);
            context.moveTo(width,height );
            context.lineTo(point["x"]+5,point["y"]+5 );
            context.strokeRect(15,15,50,80)
            context.text(""+gear,20,80 );
            context.font= "25px sans-serif"
            var numangle=0;
            for(var i=0;i<=200;i=i+10){
                numangle=(Math.PI/2)*i/200;
                context.text(""+i,rec0.width-rec0.width*(Math.cos(numangle)),rec0.height-rec0.height*(Math.sin(numangle))+20 );
            };
            context.stroke();
            context.beginPath();
            arrpoints[0]={"x":rec0.width-rec0.width*(Math.cos(rpmangle))/2,"y":rec0.height-rec0.height*(Math.sin(rpmangle))/2 };
            point = arrpoints[0]
            context.strokeStyle="red";
            context.moveTo(width,height );

            context.lineTo(point["x"]+5,point["y"]+5 );
            context.stroke();


        }


        MouseArea {
            id: mymouse
            anchors.fill: parent

            onClicked: {
                arrpoints[0]={"x": mouseX, "y": mouseY}

                rec0.requestPaint()
            }
        }
    }




    Map {

        onZoomLevelChanged: {
            //dotsize=20*(20-map.zoomLevel)
            //mapcircle is in meters, better use quickmapitem dot for dynamic size
        }


        MapCircle {
            id: circle
            visible: true
            radius: dotsize
            border.width: 1
            color: 'green'
            center: nowposition


        }

        visible: mapVisibleKey

        id: map
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        plugin: mapPlugin
        center: nowposition
        zoomLevel: 14
        property geoCoordinate startCentroid
        bearing: heading
        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                                 map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                             }
            onScaleChanged: (delta) => {
                                map.zoomLevel += Math.log2(delta)
                                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)


                            }
            onRotationChanged: (delta) => {
                                   map.bearing -= delta
                                   map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                               }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
            // and we don't yet distinguish mice and trackpads on Wayland either
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
        }
        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
        }
    }
}
