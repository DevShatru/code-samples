// Copyright (C) All rights reserved. Shatrujit Kumar, 2022.

// Parameters for poison attack
public class Poison {

    public Poison(int duration, int tick, int damagePerTick)
    {
        _duration = duration;
        _tick = tick;
        _damagePerTick = damagePerTick;
    }

    public int Duration => _duration;
    public int Tick => _tick;
    public int DPT => _damagePerTick;

    private int _duration;
    private int _tick;
    private int _damagePerTick;
}

public class PoisonAttack : Attack
{
    [Header("Attack Stats")]
    [SerializeField] private float _chargingTime = 2f;
    [SerializeField] private float _projectileSpeed = 10f;
    [SerializeField] private Transform _mouthTransform;
    [SerializeField] private GameObject _poisonProjectile;

    [Header("Poison Stats")]
    [SerializeField] private int _poisonDuration = 3;
    [SerializeField] private int _poisonTick = 1;
    [SerializeField] private int _poisonDamagePerTick = 1;
    [SerializeField] private Color _poisonColor;

    private float chargeStartTime;
    private CharacterMovement _characterMovement;
    private PlayerController _target;
    private Poison _appliedPoison;

    // Create and cache an instance of Poison, player controller, and own movement component
    private void Start()
    {
        _appliedPoison = new Poison(_poisonDuration, _poisonTick, _poisonDamagePerTick);
        _target = FindObjectOfType<PlayerController>();
        _characterMovement = GetComponent<CharacterMovement>();
    }

    // Start attack cycle
    public override void TryAttack()
    {
        if (!_canAttack || _isAttacking) return;

        _isAttacking = true;
        chargeStartTime = Time.time;
    }

    protected override void FixedUpdate()
    {
        // Early return if not in attack cycle or it hasn't completed
        if (_healthManager.IsDead)
        {
            _canAttack = false;
            return;
        }
        if (!IsAttacking || !_canAttack)
        {
            return;
        }

        if (Time.time - chargeStartTime < _chargingTime)
        {
            return;
        }
        
        // Play audio and reset attack cycle
        if (_attackAudio != null) _attackAudio.Play();
        _isAttacking = false;
        _lastAttackTime = Time.time;

        // Spawn a projectile to fire at the player
        GameObject poisonBlob = Instantiate(_poisonProjectile, _mouthTransform.position, Quaternion.LookRotation(transform.forward, transform.up));

        PoisonProjectile spawnedProjectile = poisonBlob.GetComponent<PoisonProjectile>();
        spawnedProjectile.AppliedPoison = _appliedPoison;

        Vector3 horizontalDirection = RemoveVertical(_target.transform.position - _mouthTransform.position);

        // Calculate gravity so the projectile always hits the ground after traveling the distance to the target
        float distanceScalar = horizontalDirection.magnitude;
        float height = Vector3.Dot(_mouthTransform.position - transform.position, transform.up);
        float gravity = 2 * height * Mathf.Pow(_projectileSpeed, 2) / Mathf.Pow(distanceScalar, 2);
        
        // Set gravity value on the projectilve
        spawnedProjectile.GravityVector = gravity * _characterMovement.GravityDirection;
        poisonBlob.GetComponent<Rigidbody>().velocity = _projectileSpeed * horizontalDirection.normalized;
    }

    // Remove vertical component of a vector using the transform's up direction
    private Vector3 RemoveVertical(Vector3 input)
    {
        float verticalScalar = Vector3.Dot(input, transform.up);
        return input - verticalScalar * transform.up;
    }
}
