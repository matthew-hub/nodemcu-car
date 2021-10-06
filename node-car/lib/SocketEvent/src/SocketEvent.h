#include <CarControl.h>
#include <WebSocketsClient.h>

#define USE_SERIAL Serial

#ifndef SOCKETEVENT_H
#define SOCKETEVENT_H

class SocketEvent {
   public:
    SocketEvent(CarControl& graph, WebSocketsClient& webSocket);
    void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    void carGo();
    // using Combiner = void (SocketEvent::*)(void);

   private:
    void sendMessage(String& msg);
    CarControl& graph;
    WebSocketsClient& webSocket;
};
#endif