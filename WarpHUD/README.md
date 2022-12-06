# Warp HUD and Scene

Sample showing the use of a material function to warp UVs based off scalar displacement and distortion inputs.
<br/>
![UV Curve Function](./UVCurveFunction.PNG)
<br/>

We used this function to sample the scene as a texture and warp it's UVs, creating an effect similar to the curvature/distortion that comes from wearing a glass helmet. This material was applied as a post processing effect attached to the camera.
<br/>
![Scene Warping Material](./SceneWarpPostProcess.PNG)
<br/>

We also use this function on the helmet outline material that gets drawn on the HUD, but tuned the parameters on it's material instance to provide a slightly less exagerated effect.
<br/>
![HUD Warping Material](./HUDWarpMaterial.PNG)
<br/>

## Demo
![UV Warp Demo](./UVWarp-Demo.gif)

## Reference
[Ryan Laley's Curved HUD effect tutorial](https://www.youtube.com/watch?v=-8lWA1aaSCc)