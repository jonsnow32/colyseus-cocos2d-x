"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var Map = (function () {
    function Map() {
        this.obstacles = [[100, 50], [100, 50], [100, 50], [100, 50], [100, 50], [100, 50], [100, 50], [100, 50], [100, 50], [100, 50], [100, 50], [100, 50]];
        this.width = 0;
        for (var _i = 0, _a = this.obstacles; _i < _a.length; _i++) {
            var entry = _a[_i];
            console.log(entry); // 1, "string", false
            this.width += entry[0];
        }
    }
    return Map;
}());
exports.Map = Map;
