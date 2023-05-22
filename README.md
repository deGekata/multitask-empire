# Multitask-Empire
### Simple fighting game written in C++

# Dependences 
  + sfml
  + fmt
  + make
  + clang++

# Installation 
  ``` $ git clone https://github.com/deGekata/multitask-empire.git ``` <br/>
  ``` $ cd multitask-empire ``` <br/>
  ``` $ make menu ``` <br/>
  ``` $ make run ``` <br/>

# Details of implementation
We also use so-called ECS concept in order to implement gameplay mechanics, thus making
it much less time-consuming for developpers.

ECS follows the principle of composition over inheritance, meaning that every
entity is defined not by a type hierarchy, but by the components that are associated with it.
Systems act globally over all entities which have the required components.

# Overview

## Entity
An entity represents a general-purpose object. In a game engine context, for example, every coarse
game object is represented as an entity. In our implementation, it is only a wrapper of a unique id.

## Component
A component labels an entity as possessing a particular aspect, and holds the data needed to model that aspect.
For example, every game object that can take damage might have a Health component 
associated with its entity. In our implementations we use structs to define components.

## System
A system is a process which acts on all entities with the desired components. For example,
a physics system may query for entities having mass, velocity and position components, and
iterate over the results doing physics calculations on the sets of components for each entity.

# Stdlike
Throughout all of the project we use our own implementation od such standart classes as vector, function etc.

# Config System
You can use our config system in order to implement your own custom character.
Check these example for better understanding: [knight](https://github.com/deGekata/multitask-empire/blob/master/knight.wtf),
[orc](https://github.com/deGekata/multitask-empire/blob/master/orc.wtf).

# Game controls
  + W - Jump <br/>
![](https://github.com/deGekata/multitask-empire/blob/master/assets/gifs/jump.gif)
  + A/D - Move left/right <br/>
![](https://github.com/deGekata/multitask-empire/blob/master/assets/gifs/walk.gif)
  + Tab - change special ability <br/>
![](https://github.com/deGekata/multitask-empire/blob/master/assets/gifs/spells.gif)
  + F - use of special ability <br/>
![potion](https://github.com/deGekata/multitask-empire/blob/master/assets/gifs/potion.gif)
![fireball](https://github.com/deGekata/multitask-empire/blob/master/assets/gifs/fireball.gif)
![slime](https://github.com/deGekata/multitask-empire/blob/master/assets/gifs/slime.gif)
  + B - block (only works when standing) <br/>
![potion](https://github.com/deGekata/multitask-empire/blob/master/assets/gifs/block.gif)

# Special abilities


# Rules
  + The only rule - you should kill the knight!
