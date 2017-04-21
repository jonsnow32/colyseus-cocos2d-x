"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
var colyseus_1 = require("colyseus");
var state_1 = require("../modules/state");
var world_1 = require("../modules/world");
var maps = require("../maps/map.json");
var Command;
(function (Command) {
    Command[Command["DEPART"] = 1] = "DEPART";
    Command[Command["ATTACK"] = 2] = "ATTACK";
    Command[Command["SEND_ATTACK_TO_ONOTHER"] = 3] = "SEND_ATTACK_TO_ONOTHER";
})(Command || (Command = {}));
;
var GameRoom = (function (_super) {
    __extends(GameRoom, _super);
    function GameRoom(options) {
        var _this = _super.call(this, options) || this;
        //this.setPatchRate( 1000 );
        var world = new world_1.World(maps);
        _this.setState(new state_1.State(world));
        console.log("GameRoom created!", options);
        console.log("maps" + JSON.stringify(maps));
        return _this;
    }
    GameRoom.prototype.requestJoin = function (options) {
        return (this.clients.length < 5);
    };
    GameRoom.prototype.onJoin = function (client) {
        this.state.addPlayer(client.id);
    };
    GameRoom.prototype.onLeave = function (client) {
        this.state.removePlayer(client.id);
    };
    GameRoom.prototype.onMessage = function (client, message) {
        console.log(message, " GameRoom received from", client.id);
        //this.state.onMessage(client.id,data);
        //attack to clientID //param //from/to/nSatellite
        var command = message[0];
        var data = message[1];
        if (command == Command.DEPART) {
            this.state.world.depart(message[1], message[2], message[3]);
            this.sendAllExcept(client.id, [Command.SEND_ATTACK_TO_ONOTHER, message[1], message[2], message[3]]);
        }
        //depart params : fromPlanet ToPlanet mSatellite 
        if (command == Command.ATTACK) {
            this.state.world.attack(message[1], message[2], message[3]);
        }
    };
    GameRoom.prototype.sendAllExcept = function (clientID, data) {
        console.log("sendAllExcept clientID :" + clientID);
        for (var i = 0; i < this.clients.length; i++) {
            console.log("sendAllExcept :" + this.clients[i].id);
            if (this.clients[i].id != clientID) {
                this.send(this.clients[i], data);
            }
        }
    };
    GameRoom.prototype.update = function () {
        this.state.update();
    };
    GameRoom.prototype.onDispose = function () {
        console.log("Dispose GameRoom");
    };
    return GameRoom;
}(colyseus_1.Room));
exports.GameRoom = GameRoom;
