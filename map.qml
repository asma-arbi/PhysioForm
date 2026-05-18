import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

Rectangle {
    id: window

    property double oldLat: 36.8065
    property double oldLng: 10.1815
    property Component comMarker: mapMarker

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(oldLat, oldLng);
        zoomLevel: 6

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (mouse.button === Qt.LeftButton && mouse.modifiers === Qt.ControlModifier) {
                    var coordinate = mapView.toCoordinate(Qt.point(mouse.x, mouse.y));
                    console.log("Double clicked at:", coordinate.latitude, coordinate.longitude);
                    geocode(coordinate.latitude, coordinate.longitude);
                }
            }
        }

        property string locationName: "" // Propriété pour stocker le nom de l'emplacement

        Text {
            text: mapView.locationName // Liaison à la propriété pour afficher le nom de l'emplacement
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    // Fonction de géocodage pour obtenir le nom de l'emplacement en fonction des coordonnées
    function geocode(latitude, longitude) {
        var xhr = new XMLHttpRequest();
        var url = "https://nominatim.openstreetmap.org/reverse?format=json&lat=" + latitude + "&lon=" + longitude + "&zoom=18&addressdetails=1";

        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    var response = JSON.parse(xhr.responseText);
                    window.mapView.locationName = response.display_name; // Mettre à jour la propriété avec le nom de l'emplacement
                } else {
                    console.error("Erreur lors de la récupération des données de géocodage:", xhr.status);
                }
            }
        }

        xhr.open("GET", url, true);
        xhr.send();
    }

    function setCenter(lat, lng) {
        mapView.pan(oldLat - lat, oldLng - lng)
        oldLat = lat
        oldLng = lng
    }

    function addMarker(lat, lng) {
        var item = comMarker.createObject(window, {
                                           coordinate: QtPositioning.coordinate(lat, lng)
                                          })
        mapView.addMapItem(item)
    }

    Component {
        id: mapMarker
        MapQuickItem {
            id: markerImg
            anchorPoint.x: image.width/4
            anchorPoint.y: image.height
            coordinate: position

            sourceItem: Image {
                id: image
                source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
            }
        }
    }
}
