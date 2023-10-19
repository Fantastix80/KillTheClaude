#pragma once
#ifndef ATTACK_H
#define ATTACK_H

struct Type;

struct Attack
{
	char Name[20];
	float Damage;
	struct Type* Type;
};

#endif ATTACK_H