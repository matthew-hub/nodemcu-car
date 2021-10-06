#include <CarControl.h>
#include <WebSocketsClient.h>

// TODO: handle if someone connect to car

#define USE_SERIAL Serial

#ifndef SOCKETEVENT_H
#define SOCKETEVENT_H

class SocketEvent {
   public:
    SocketEvent(CarControl& car, WebSocketsClient& webSocket);
    void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);

   private:
    void sendMessage(String& msg);
    void handleEvent(String payload);
    CarControl& car;
    WebSocketsClient& webSocket;
};
#endif