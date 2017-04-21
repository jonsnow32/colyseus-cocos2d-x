import { Room } from "colyseus";
export declare class GameRoom extends Room<any> {
    constructor(options: any);
    requestJoin(options: any): boolean;
    onJoin(client: any): void;
    onLeave(client: any): void;
    onMessage(client: any, message: any): void;
    sendAllExcept(clientID: any, data: any): void;
    update(): void;
    onDispose(): void;
}
