"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var PLANET_STATE;
(function (PLANET_STATE) {
    PLANET_STATE[PLANET_STATE["FREE"] = 0] = "FREE";
    PLANET_STATE[PLANET_STATE["BUILDING"] = 1] = "BUILDING";
    PLANET_STATE[PLANET_STATE["SPWAN"] = 2] = "SPWAN";
})(PLANET_STATE = exports.PLANET_STATE || (exports.PLANET_STATE = {}));
;
var Planet = (function () {
    function Planet(x, y, maxSatellite, player, team) {
        var _this = this;
        this.x = x;
        this.y = y;
        this.team = team;
        this.playerID = player;
        this.maxSatellite = maxSatellite;
        this.spwanSpeed = 10.0 / maxSatellite;
        this.canInit = false;
        this.nSatellite = 1;
        this.currentState = PLANET_STATE.FREE;
        setInterval(function () {
            if (_this.currentState == PLANET_STATE.SPWAN && _this.nSatellite < _this.maxSatellite) {
                _this.nSatellite++;
                console.log("nSatellite= " + _this.nSatellite);
            }
        }, this.spwanSpeed * 1000);
    }
    Planet.prototype.reset = function () {
        this.playerID = "";
        this.nSatellite = 1;
    };
    Planet.prototype.upgrade = function () {
    };
    Planet.prototype.depart = function (toPlanet) {
        var temp = this.nSatellite;
        this.nSatellite = 1;
        return temp;
    };
    Planet.prototype.onDamge = function (fromPlanet, nSatellite) {
        if (this.currentState == PLANET_STATE.FREE) {
            this.nSatellite += nSatellite;
            this.playerID = fromPlanet.playerID;
            this.currentState = PLANET_STATE.BUILDING;
        }
        else if (this.currentState == PLANET_STATE.BUILDING) {
            if (this.playerID == fromPlanet.playerID)
                this.nSatellite += nSatellite;
            else
                this.nSatellite -= nSatellite;
            if (this.nSatellite <= 0)
                this.playerID = fromPlanet.playerID;
            this.nSatellite = Math.abs(this.nSatellite);
            if (this.nSatellite >= this.maxSatellite) {
                this.currentState = PLANET_STATE.SPWAN;
            }
        }
        else if (this.currentState == PLANET_STATE.SPWAN) {
            if (this.playerID == fromPlanet.playerID)
                this.nSatellite += nSatellite;
            else
                this.nSatellite -= nSatellite;
            if (this.nSatellite <= 0)
                this.playerID = fromPlanet.playerID;
            this.nSatellite = Math.abs(this.nSatellite);
        }
    };
    Planet.prototype.getCurrentState = function () {
        return this.currentState;
    };
    return Planet;
}());
exports.Planet = Planet;
