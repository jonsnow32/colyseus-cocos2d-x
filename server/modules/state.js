"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var player_1 = require("./player");
var State = (function () {
    function State(world) {
        this.world = world;
        this.players = {};
        this.team = { "red": "", "blue": "", "green": "", "yello": "" };
    }
    State.prototype.addPlayer = function (clientID) {
        for (var key in this.team) {
            if (this.team[key] == "") {
                this.team[key] = clientID;
                var player = new player_1.Player(clientID, this.getColor(key));
                this.players[clientID] = player;
                this.world.findPlanetEmptyFor(clientID);
                break;
            }
        }
    };
    State.prototype.getColor = function (color) {
        if (color == "red")
            return 1;
        if (color == "blue")
            return 2;
        if (color == "green")
            return 3;
        if (color == "yellow")
            return 4;
        return 0;
    };
    State.prototype.removePlayer = function (clientID) {
        delete this.players[clientID];
        for (var key in this.team) {
            if (this.team[key] == clientID)
                this.team[key] = "";
        }
        this.world.removeOwnerPlanet(clientID);
    };
    return State;
}());
exports.State = State;
