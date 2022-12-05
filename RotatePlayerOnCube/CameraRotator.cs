public class CameraRotator : MonoBehaviour
{
    [Header("Rotation Settings")]
    [SerializeField, Range(0f, 50f)] private float _cameraSpeed = 10f;
    [SerializeField] private CharacterMovement3D _characterMovement;
    private Quaternion _initialRotation;
    private Quaternion _targetRotation;

    private void Awake()
    {
        // Cache initial relative rotation, and character movement script reference
        _initialRotation = transform.rotation;
        if(_characterMovement == null) _characterMovement = transform.parent.GetComponentInChildren<CharacterMovement3D>();
    }

    private void Start()
    {
        // Subscrive to Rotator even on character movement
        if(_characterMovement != null) _characterMovement.Rotator += Rotate;
    }

    // Calculate target rotation on event
    public void Rotate(Quaternion rotation)
    {
        _targetRotation = rotation * _initialRotation;
    }

    // Slerp rotation to target value if it's different from current rotation
    private void FixedUpdate()
    {
        if (transform.rotation == _targetRotation) return;
        transform.rotation = Quaternion.Slerp(transform.rotation, _targetRotation, Time.fixedDeltaTime * _cameraSpeed);
    }
}
