# Meeting Notes
In this file, you are required to take notes for your weekly meetings. 
In each meeting, you are required to discuss:

1. What each member has done during the week?
2. Are there challenges or problems? Discuss the possible solutions
3. Plan for the next week for everyone
4. Deviations and changes to the project plan, if any


# Meeting 18.10.2023

**Participants**: 
1. Aleksi Kalsta
2. Eetu Reijonen
3. Eemeli Forsbom
4. Henri-Mikael Suominen

## Summary of works
1. All
Getting to know what we have to do for project plan. Testing that Git repo works for all.
Went through which things we want to implement:
- All basic features
- From recommended: All but advanced materials
- From advanced: We will check if we have time when we proceed

Planned which file format we want to use for 3D scenes. Possible options:
- XML
- YML
- Universal scene description

Switched some ideas on high level structure of the program.

Settled the time for meetings to be 14:15-16:00 on mondays.

## Challenges

1. Lack of knowledge

## Actions
1. For all
- Gather more knowledge on the subject

## Project status 
First meeting held.

### TODOs

# Meeting 23.10.2023

**Participants**: 
1. Aleksi Kalsta
2. Eetu Reijonen
3. Eemeli Forsbom
4. Henri-Mikael Suominen

## Summary of works
1. Everybody - familiarizing with the topic, project plan formulation during the meeting
2. Eetu - Julia demo

## Challenges

## Actions
1. Start working on the code
2. Finish project plan

## Project status 
Julia demo implementation exists. 

### TODOs
1. Aleksi - software structure -part of project report, Interface part of first implementation
2. Eetu - external libraries -part of project report, core Renderer part of first implementation
3. Eemeli - division of work and schedule -parts of project report, Scene and related objects part of first implementation
4. Henri - scope of work -part of project report, Fileloader part of first implementation

# Meeting 30.10.2023

**Participants**: 
1. Aleksi Kalsta
2. Eetu Reijonen
3. Henri-Mikael Suominen

## Summary of works
1. Aleksi - Got the SFML library to work with CMake and got the CMake file working seamesly
2. Everybody - Choosing and getting familiar with exteranl libraries

## Challenges
1. Getting libraries to work with CMake
2. CMake in general

## Actions
1. Start actually working on the code after starting hurdles

## Project status 
First implementation work in progress

### TODOs
1. Aleksi - Continuation of Interface class implementation
2. Eetu - Porting Julia code for Renderer class
3. Eemeli - Implementation of Scene and related objects classes
4. Henri - Continuation of Fileloader implementation

# Meeting 6.11.2023

**Participants**: 
1. Aleksi Kalsta
2. Eetu Reijonen
3. Henri-Mikael Suominen

## Summary of works
1. Eetu - Renderer and partial Object, Ball and Scene implementations
2. Aleksi - Interface implementation and bug fixes
3. Henri - partial FileLoader implementation

## Challenges
* Use of external libraries (yaml)

## Actions
* Complete basic features except for triangle meshes
* Add support for triangle meshes
* Print methods for objects
* Renderer status messages to console
* Start designing basic unit tests (testing libraries?)

## Project status 
Initial implementation of the program exists. The programmer can input scene parameters into the code and then render the scene.

### TODOs
1. Aleksi - integrate scene input into Interface, start looking into unit testing
2. Eetu - create print methods, complete classes, fix includes, add environment lighting, optimize rendering code (and comment everything)
3. Eemeli - start working on triangle meshes (.obj files)
4. Henri - complete yaml scene input