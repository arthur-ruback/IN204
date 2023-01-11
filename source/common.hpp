#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

struct Message
{
    sf::Uint32 sender;
    sf::Uint32 dest;
    std::string content;
};

sf::Packet& operator <<(sf::Packet& packet, const Message& message)
{
    return packet << message.sender << message.dest << message.content;
}

sf::Packet& operator >>(sf::Packet& packet, Message& message)
{
    return packet >> message.sender >> message.dest >> message.content;
}

std::ostream& operator << (std::ostream& aStream, const Message& message)
{
    return aStream << "FROM:" << message.sender << " TO:" << message.dest << " CONTENT: " << message.content;
}