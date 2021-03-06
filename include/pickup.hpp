#ifndef PICKUP_H
#define PICKUP_H

class Item;
class Actor;
class Ammo;
class Wpn;

namespace item_pickup
{

//Can always be called to check if something is there to be picked up.
void try_pick();

void try_unload_wpn_or_pickup_ammo();

Ammo* unload_ranged_wpn(Wpn& wpn);

} //Item_pickup

#endif
