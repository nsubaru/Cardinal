export module Cardinal.Time.Internals;

export import Cardinal.Time;

export namespace Cardinal::Internals
{
	InternalApiCall void ConvertDateTimeToStringImpl(const Time::DateTime& time, CharStr buffer);
	InternalApiCall void ConvertTimeSpanToStringImpl(const Time::DateSpan& time, CharStr buffer);
	InternalApiCall void ConvertDateToStringImpl(Time::Date date, CharStr buffer);
}