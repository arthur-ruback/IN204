#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#define SERVER_ID 0

#define MSG_STD      0
#define MSG_CONX_REQ 1
#define MSG_CONX_REP 2
#define MSG_LINK_REQ 3
#define MSG_LINK_REP 4

#define EMITTER 0
#define RECIEVER 1

struct ClientData {
    sf::TcpSocket* socket;
    unsigned id;
    bool type;
    unsigned link;
};

struct Message
{
    sf::Uint32 msgType;
    sf::Uint32 sender;
    sf::Uint32 dest;
    std::string content;
};

sf::Packet& operator <<(sf::Packet& packet, const Message& message)
{
    return packet << message.msgType << message.sender << message.dest << message.content;
}

sf::Packet& operator >>(sf::Packet& packet, Message& message)
{
    return packet >> message.msgType >> message.sender >> message.dest >> message.content;
}

std::ostream& operator << (std::ostream& aStream, const Message& message)
{
    return aStream << "[" << message.msgType << "] FROM:" << message.sender << " TO:" << message.dest << " CONTENT: " << message.content;
}

struct AuthMessage {
    sf::Uint16 msgType;
    sf::Uint32 sender;
    sf::Uint32 dest;
    std::string content;
    sf::Uint32 authentication;
};

sf::Packet& operator <<(sf::Packet& packet, const AuthMessage& message)
{
    return packet << message.msgType << message.sender << message.dest << message.content << message.authentication;
}

sf::Packet& operator >>(sf::Packet& packet, AuthMessage& message)
{
    return packet >> message.msgType >> message.sender >> message.dest >> message.content >> message.authentication;
}

std::ostream& operator << (std::ostream& aStream, const AuthMessage& message)
{
    return aStream << "[" << message.msgType << "]FROM:" << message.sender << " TO:" << message.dest <<
         " CONTENT: " << message.content << " AUTH: " << message.authentication;
}