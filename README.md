# colyseus-cocos2d-x
Multiplayer Game for Nodejs base on [Colyseus](https://github.com/gamestdio/colyseus)

### Installation
Download and copy cocos2d-x [v3.15+](http://cocostudio.download.appget.cn/Cocos2D-X/cocos2d-x-3.15.zip) to {$ROOT_FOLDER}/client/cocos2d

### Win32 version
Run {$ROOT_FOLDER}/client/proj.win32 (tested on visual studio 2015)

## How to run server
Ensure you have [Node v6+](http://nodejs.org/) installed. Then run these
commands in your commandline:

```
cd server
npm install
npm start
```

## How to Client connect your server
open file {$ROOT_FOLDER}/client/Classes/GamePlay/SceneBattle.cpp
```
bool SceneBattle::init()
{
.....
	Client::getInstance()->start("ws://13.76.179.224:2667", CC_CALLBACK_1(SceneBattle::onConnectToServer, this));
.....
}
replace ws://13.76.179.224:2667 to ws://localhost:2667
```

