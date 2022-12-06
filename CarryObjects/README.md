# Carry Objects

Sample demonstrating how M10 Minutes to Breathe handled physics interactions when carrying objects.<br/>
On pick up, the item had gravity disabled, velocity reset, and collision changed to ignore the player. We also bound a delegate that would force the player to drop the object if it collided with anything else.<br/>
On release, we re-enable gravity and collision with the player, and remove our bound delegate to drop the object. We also flip a bool to ensure the object reports a noise event on it's first pitch after release<br/>
<br/>
![Carry Objects Demo](./CarryObjects-Demo.gif)
