public class CharacterMovement3D : CharacterMovement
{
    public delegate void RotationDelegate(Quaternion rotation);
    public RotationDelegate Rotator;

    private void FixedUpdate()
    {
        if (_canSwitchWalls && CheckMoveToSurface())
        {
            // Check if the surface we struck is along the right axis
            float roundedDotRight = Mathf.Round(Vector3.Dot(_surfaceHit.normal, _right));

            // Check if moving on forward axis
            float roundedDotForward = Mathf.Round(Vector3.Dot(_surfaceHit.normal, _forward));

            // Update character params based on _surfaceHit calculations
            UpdateOnSurface(roundedDotRight, roundedDotForward);
            _lastSwitchTime = Time.time;
        }
    }

    private void UpdateOnSurface(float roundedDotRight, float dotForward)
    {
        // Update gravity and look direction
        _gravityDirection = -_surfaceHit.normal;
        LookDirection = transform.up;
        
        // Transform character's rotation and position
        Quaternion newRotation = Quaternion.LookRotation(transform.up, _surfaceHit.normal);
        transform.rotation = newRotation;
        transform.position = _surfaceHit.point - _gravityDirection * _gridSize;
        
        if (roundedDotRight != 0)
        {

            // Find new _right and _gravityDirection
            _right = roundedDotRight * Vector3.Cross(_right, _forward);

            // Fire rotator delegate to update the camera
            Rotator(newRotation * Quaternion.Euler(0f, roundedDotRight * 90f, 0f));
            return;
        }
        
        if (roundedDotForward != 0)
        {
            // Find new _forward and _gravityDirection
            _forward = roundedDotForward * _gravityDirection;

            // Fire rotator delegate to update the camera
            Rotator(newRotation * ((roundedDotForward == -1)?Quaternion.identity:Quaternion.Euler(0f, 180f, 0f)));
        }
    }

    public override bool CheckMoveToSurface()
    {
        // Don't switch if we're not moving
        if (!HasMoveInput || IsDashing) return false;

        // Cooldown
        if (Time.time - _lastSwitchTime < _wallSwitchCooldown) return false;

        return CheckForWall(_wallSwitchOffset);
    }

    private bool CheckForWall(float distance)
    {
        // Offset raycast height so we don't hit the floor
        Vector3 origin = transform.position - _gravityDirection * _gridSize / 2;
        
        // Raycast to find a wall
        bool hit = Physics.Raycast(origin, transform.forward, out _surfaceHit, distance, _groundMask);
        return hit;
    }
}