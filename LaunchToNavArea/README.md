# Off Navigation Area Jump

Sample illustrating a catch-all AI task that executes when the unit is not on any part of the navigation area. The task finds a random navigable point within a given radius, calculates the launch velocity required to reach that point, and launches the AI's pawn. After a duration, it checks for completion and reattempts the launch until a success.
<br/>

## Demo
In the demo, the orange region represents the off-nav area. Our unit fails to reach the nav area on it's first jump, but tries again after half a second and succeeds.
<br/>
![Off Nav Jump Demo](./LaunchToNav-Demo.gif)
