"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var planet_1 = require("./planet");
var planet_2 = require("./planet");
var shortid = require("shortid");
var World = (function () {
    function World(map) {
        this.currentMapID = 0;
        this.planets = {};
        var currentMap = map.layers[this.currentMapID++].objects;
        this.width = map.tilewidth * map.width;
        this.height = map.tileheight * map.height;
        for (var i = 0; i < currentMap.length; i++) {
            if (currentMap[i].visible == true) {
                var plt = new planet_1.Planet(currentMap[i].x + currentMap[i].width / 2, this.height - currentMap[i].y - currentMap[i].height / 2, currentMap[i].width / 2, "", -1);
                if (currentMap[i].type == "PlayerInit")
                    plt.canInit = true;
                var planetID = shortid.generate();
                this.planets[planetID] = plt;
                console.log(planetID);
            }
        }
    }
    World.prototype.findPlanetEmptyFor = function (clientID) {
        for (var key in this.planets) {
            if (this.planets[key].canInit && this.planets[key].playerID == "") {
                this.planets[key].playerID = clientID;
                this.planets[key].currentState = planet_2.PLANET_STATE.SPWAN;
                console.log("XYZ");
                break;
            }
        }
    };
    World.prototype.removeOwnerPlanet = function (clientID) {
        for (var key in this.planets) {
            if (this.planets[key].playerID == clientID)
                this.planets[key].reset();
        }
    };
    World.prototype.depart = function (fromPlanetKeys, toPlanetKeys, nSatellite) {
        var src = this.planets[fromPlanetKeys];
        var des = this.planets[toPlanetKeys];
        var realnSatellite = src.depart(des);
        if (nSatellite > realnSatellite)
            console.log("detect cheat form " + this.planets[fromPlanetKeys].playerID);
    };
    World.prototype.attack = function (fromPlanetKeys, toPlanetKeys, nSatellite) {
        var src = this.planets[fromPlanetKeys];
        var des = this.planets[toPlanetKeys];
        des.onDamge(src, nSatellite);
    };
    return World;
}());
exports.World = World;
