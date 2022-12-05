# Smart Link + Jump

Sample illustrating how AI units moved between discrete navigation areas using smart links. When a unit reaches the smart link, that fires an event which passes the smart link's destination to the unit's "Smart Link Jump" method.
<br/>
![Smart Link Reached](./SmartLinkReachedEvent.PNG)
<br/>
This method on the AI calculates the velocity required to reach the end, and launches the character using Unreal's built-in "Launch Character" method.
<br/>
![Smart Link Jump](./SmartLinkJumpMethod.PNG)
![Calculate Velocity](./CalcVelocityMethod.PNG)
<br/>

## Demo
![Smart Link Jump Demo](./SmartNavJump-Demo.gif)
