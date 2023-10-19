#pragma once
#ifndef PLAYER_H
#define PLAYER_H

struct Attack;

struct Player
{
    char Name[20];
    float Health;
    struct Attack* PlayerAttack[2];
    int X;
    int Y;
};

#endif PLAYER_H