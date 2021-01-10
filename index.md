## What is Atomic Engine 2?

Atomic Engine 2 is a game engine made in C and C++ for the 3rd assignment of the Game Engines subject in CITM. 
We created a basic 3D Engine and a Audio subsystem integrating Wwise.
We forked Marc Pagès's Genesis Engine as starting point for this assignment.
Marc Pagès's [Genesis Engine](https://github.com/marcpages2020/GenesisEngine)

## Who are we?

### [Pau Raurell](https://github.com/pauraurell)
### [Pol Galan](https://github.com/pgalmor)
![](Web%20pics/Members.PNG)

Pau Raurell contributions: Wwise integration, Module Audio, Audio Components, Reverb (code)...
Pol Galan contributions: Spatial Audio, Background music loop, Reverb (Wwise)...

## Sub-systems

We have sub-systems such as a scene serialization system that allows us to save and load scenes, mouse picking, guizmos, bounding boxes, camera culling, Game Objects that have components like Transform, Mesh, Material,
Lights, Camera...

## Audio Sub-system

For the last assignment we created a Audio sub-system integrating Wwise to our Engine.
We can load Wwise Banks from the engine and use its events to play musicm, fade in or out a sound...
This subsystem allows you to add 3 new components to a Game Object. The first one is the Audio Emitter, which is very similar to the Audio Source component
in Unity and you can add this component to a Game Object that you want to be the source of a sound or music.
Then we have the Listener component, without this one the audio that comes from the Emitter won't be listened. By default the camera has a Listener.
The last one is the Reverb Zone, done with Wwise Parameters, and it is a sphere that if contains a Audio Emitter, the sound from that emitter gets reverbed more or less depending on 
the distance to the center of the sphere.
In the demo scene there are two game objects with spatial audio, a background music and a reverb zone. 

![](Web%20pics/gif1.gif)
![](Web%20pics/gif2.gif)

## Demo Video
[![IMG](https://img.youtube.com/vi/YJ7yxsC5ZrA/0.jpg)](https://www.youtube.com/watch?v=YJ7yxsC5ZrA)

## Release
Atomic Engine 2 v1.0 Release [Click here](https://github.com/pauraurell/AtomicEngine2/releases/tag/v1.0)

## Licence

MIT License

Copyright (c) 2020 Pol Galan Morales & Pau Raurell Gomis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files Genesis Engine, to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

