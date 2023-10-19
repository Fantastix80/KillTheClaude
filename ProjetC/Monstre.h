#pragma once
#ifndef MONSTRE_H
#define MONSTRE_H

struct Type;

struct Monstre
{
    char Name[20];
    float Health;
    float Damage;
    struct Type* Type;
    int X;
    int Y;
};

#endif MONSTRE_H