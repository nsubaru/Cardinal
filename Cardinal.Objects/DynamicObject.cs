namespace Cardinal;

public class DynamicObject
{
    public object Value { get; set; }

    public DynamicObject(object value)
    {
        Value = value ?? throw new ArgumentNullException(nameof(value));
    }
}
