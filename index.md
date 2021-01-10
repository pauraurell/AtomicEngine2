## What is Atomic Engine 2?

Atomic Engine 2 is a game engine made in C and C++ for the 3rd assignment of the Game Engines subject in CITM. 
We created a basic 3D Engine and a Audio subsystem integrating Wwise.
We forked Marc Pagès's Genesis Engine as starting point for this assignment.

## Who are we?

### [Pau Raurell](https://github.com/pauraurell)
### [Pol Galan(https://github.com/pgalmor)
![Photo](https://github.com/pauraurell/AtomicEngine2/blob/gh-pages/Web%20pics/Members.PNG)

Pau Raurell contributions: Wwise integration, Module Audio, Audio Components, Reverb (code),
Pol Galan contributions: Spatial Audio, Background music loop, Reverb (Wwise),

## Sub-systems

We have sub-systems such as a scene serialization system that allows us to save and load scenes, mouse picking, guizmos, camera culling, Game Objects that have components like Transform, Mesh, Material,
Lights, Cameras...  

## Audio Sub-system

For the last assignment we created a Audio sub-system integrating Wwise to our Engine.
This subsystem allows you to add 3 new components to a Game Object. The first one is the Audio Emitter, which is very similar to the Audio Source component
in Unity and you can add this component to a Game Object that you want to be the source of a sound or music.
Then we have the Listener component, without this one the audio that comes from the Emitter won't be listened. By default the camera has a Listener.
The last one is the Reverb Zone and it is a sphere that if contains a Audio Emitter, the sound from that emitter gets reverbed more or less depending on 
the distance to the center of the sphere.
In the demo scene there are two game objects with spatial audio, a background music and a reverb zone. 