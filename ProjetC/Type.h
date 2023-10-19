#pragma once
#ifndef TYPE_H
#define TYPE_H

struct Type
{
    char Name[20];
    struct Type* TypeVulnerable[5];
    float MultiplierVulnerable;
    struct Type* TypeResistant[5];
    float MultiplierResistant;
};

#endif TYPE_H