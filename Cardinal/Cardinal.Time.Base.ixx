export module Cardinal.Time:Base;

export import Cardinal.Core;
export import Cardinal.CTTI.Minimal;

export namespace Cardinal::Time
{
	struct Date;
	struct DateSpan;
	struct TimeSpan;
	struct DateTime;

	/// <summary>
	/// Number of intervals in seconds
	/// </summary>
	constexpr UInt64 SecondInNtTime = 1000ULL * 10000ULL;

	/// <summary>
	///Number of ticks in a millisecond
	/// </summary>
	constexpr UInt64 MiliSecondsTicksCount = SecondInNtTime / 1000ULL;

	/// <summary>
	/// Number of ticks in a second
	/// </summary>
	constexpr UInt64 SecondTicksCount = MiliSecondsTicksCount * 1000ULL;

	/// <summary>
	/// Number of ticks in a minute
	/// </summary>
	constexpr UInt64 MinutesTicksCount = SecondTicksCount * 60ULL;

	/// <summary>
	/// Number of ticks in an hour
	/// </summary>
	constexpr UInt64 HourTicksCount = MinutesTicksCount * 60ULL;

	/// <summary>
	/// Number of ticks in a day
	/// </summary>
	constexpr UInt64 DayTicksCount = HourTicksCount * 24ULL;

	/// <summary>
	/// Number of ticks in a week
	/// </summary>
	constexpr UInt64 WeekTicksCount = DayTicksCount * 7ULL;

	/// <summary>
	/// Number of ticks in a month (30 days)
	/// </summary>
	constexpr UInt64 MonthTicksCount = DayTicksCount * 30ULL;

	/// <summary>
	/// Number of ticks in a year (366 days)
	/// </summary>
	constexpr UInt64 YearTicksCount = DayTicksCount * 366ULL;

	/// <summary>
	/// Date class
	/// </summary>
	struct Date final {
	public:
		/// <summary>
		/// Year
		/// </summary>
		UInt16 Year;

		/// <summary>
		/// Month
		/// </summary>
		UInt16 Month;

		/// <summary>
		/// Weekday
		/// </summary>
		UInt16 WeekDay;

		/// <summary>
		/// Day of the month
		/// </summary>
		UInt16 Day;

		/// <summary>
		/// Hour
		/// </summary>
		UInt16 Hour;

		/// <summary>
		/// Minute
		/// </summary>
		UInt16 Minutes;

		/// <summary>
		/// Second
		/// </summary>
		UInt16 Second;

		/// <summary>
		/// Millisecond
		/// </summary>
		UInt16 MiliSecond;

		/// <summary>
		/// An empty constructor, initializes the date "1601.1.1 0:0:0.0"
		/// </summary>
		ForceInline constexpr Date() :Year(1601), Month(1), WeekDay(0), Day(1), Hour(0), Minutes(0), Second(0), MiliSecond(0) {};

		/// <summary>
		/// Date and time constructor
		/// </summary>
		/// <param name="Year">Year</param>
		/// <param name="Month">Month</param>
		/// <param name="WeekDay">Weekday</param>
		/// <param name="Day">Day of the month</param>
		/// <param name="Hour">Hour</param>
		/// <param name="Minutes">Minute</param>
		/// <param name="Seconds">Second</param>
		/// <param name="MiliSeconds">Millisecond</param>
		ForceInline constexpr Date(
			UInt16 Year, UInt16 Month, UInt16 WeekDay, UInt16 Day,
			UInt16 Hour, UInt16 Minutes, UInt16 Seconds, UInt16 MiliSeconds
		) :
			Year(Year), Month(Month), WeekDay(WeekDay), Day(Day),
			Hour(Hour), Minutes(Minutes), Second(Seconds), MiliSecond(MiliSeconds) {
		};

		/// <summary>
		/// Constructor with time class object
		/// </summary>
		/// <param name="time">Time class object</param>
		Date(const DateTime& time);

		/// <summary>
		/// Constructor with time class object
		/// </summary>
		/// <param name="time">Time class object</param>
		Date(DateTime&& time);

		/// <summary>
		/// Conversion in time
		/// </summary>
		/// <returns>Time object</returns>
		operator DateTime() const;

		~Date() {};

	public:
		StructCttiDef(Date,
			DefStructFieldData(Date, Year),
			DefStructFieldData(Date, Month),
			DefStructFieldData(Date, WeekDay),
			DefStructFieldData(Date, Day),
			DefStructFieldData(Date, Hour),
			DefStructFieldData(Date, Minutes),
			DefStructFieldData(Date, Second),
			DefStructFieldData(Date, MiliSecond)
		);
	};

	/// <summary>
	/// Relative date class
	/// </summary>
	struct DateSpan final {
	public:
		/// <summary>
		/// Year
		/// </summary>
		UInt16 Year;

		/// <summary>
		/// Month
		/// </summary>
		UInt16 Month;

		/// <summary>
		/// Number of weeks
		/// </summary>
		UInt16 Weeks;

		/// <summary>
		/// Day of the month
		/// </summary>
		UInt16 Day;

		/// <summary>
		/// Hour
		/// </summary>
		UInt16 Hour;

		/// <summary>
		/// Minute
		/// </summary>
		UInt16 Minutes;

		/// <summary>
		/// Second
		/// </summary>
		UInt16 Second;

		/// <summary>
		/// Millisecond
		/// </summary>
		UInt16 MiliSecond;

		/// <summary>
		/// Number of ticks
		/// </summary>
		UInt16 Ticks;

		/// <summary>
		/// Date and time constructor
		/// </summary>
		/// <param name="Year">Year</param>
		/// <param name="Month">Month</param>
		/// <param name="Weeks">Number of weeks</param>
		/// <param name="Day">Day of the month</param>
		/// <param name="Hour">Hour</param>
		/// <param name="Minutes">Minute</param>
		/// <param name="Seconds">Second</param>
		/// <param name="MiliSeconds">Millisecond</param>
		/// <param name="Ticks">Number of ticks</param>
		ForceInline constexpr DateSpan(
			UInt16 Year = 0,
			UInt16 Month = 0,
			UInt16 Weeks = 0,
			UInt16 Day = 0,
			UInt16 Hour = 0,
			UInt16 Minutes = 0,
			UInt16 Seconds = 0,
			UInt16 MiliSeconds = 0,
			UInt16 Ticks = 0
		) :
			Year(Year),
			Month(Month),
			Weeks(Weeks),
			Day(Day),
			Hour(Hour),
			Minutes(Minutes),
			Second(Seconds),
			MiliSecond(MiliSeconds),
			Ticks(Ticks) {
		};

		/// <summary>
		/// Constructor with time class object
		/// </summary>
		/// <param name="time">Time class object</param>
		DateSpan(const DateTime& time);

		/// <summary>
		/// Conversion in time
		/// </summary>
		/// <returns>Time object</returns>
		operator DateTime() const;

	public:
		StructCttiDef(DateSpan,
			DefStructFieldData(DateSpan, Year),
			DefStructFieldData(DateSpan, Month),
			DefStructFieldData(DateSpan, Weeks),
			DefStructFieldData(DateSpan, Day),
			DefStructFieldData(DateSpan, Hour),
			DefStructFieldData(DateSpan, Minutes),
			DefStructFieldData(DateSpan, Second),
			DefStructFieldData(DateSpan, MiliSecond),
			DefStructFieldData(DateSpan, Ticks)
		);
	};

	/// <summary>
	/// Timer interval class
	/// </summary>
	struct TimeSpan final {
	public:
		UInt64 Ticks;

	public:
		/// <summary>
		/// Costructor that initializes the zero segment
		/// </summary>
		ForceInline constexpr TimeSpan() : Ticks(0) {}

		/// <summary>
		/// Constructor with number of ticks that creates the segment with specified number of ticks
		/// </summary>
		/// <param name="ticks">Number of ticks</param>
		ForceInline constexpr TimeSpan(UInt64 ticks) : Ticks(ticks) {}

		/// <summary>
		/// Constructor with data segment that creates the segment with specified data interval
		/// </summary>
		/// <param name="dateSpan">Data interval</param>
		ForceInline constexpr TimeSpan(DateSpan dateSpan) : Ticks(0) {
			Ticks += dateSpan.MiliSecond * MiliSecondsTicksCount;
			Ticks += dateSpan.Second * SecondTicksCount;
			Ticks += dateSpan.Minutes * MinutesTicksCount;
			Ticks += dateSpan.Hour * HourTicksCount;
			Ticks += dateSpan.Day * DayTicksCount;
			Ticks += dateSpan.Weeks * WeekTicksCount;
			Ticks += dateSpan.Month * MonthTicksCount;
			Ticks += dateSpan.Year * YearTicksCount;
		}

	public:
		/// <summary>
		/// Adds ticks (if negative - substracts)
		/// </summary>
		/// <param name="ticksCount">Number of ticks</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddTicks(Int32 ticksCount) {
			if (ticksCount < 0)
			{
				Ticks -= (-ticksCount);
			}
			else
			{
				Ticks += ticksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds milliseconds (if negative - substracts)
		/// </summary>
		/// <param name="miliSeconds">Number of milliseconds</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddMiliSeconds(Int32 miliSeconds) {
			if (miliSeconds < 0)
			{
				Ticks -= (-miliSeconds) * MiliSecondsTicksCount;
			}
			else
			{
				Ticks += miliSeconds * MiliSecondsTicksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds seconds (if negative - substracts)
		/// </summary>
		/// <param name="seconds">Number of seconds</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddSeconds(Int32 seconds) {
			if (seconds < 0)
			{
				Ticks -= (-seconds) * SecondTicksCount;
			}
			else
			{
				Ticks += seconds * SecondTicksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds minutes (if negative - substracts)
		/// </summary>
		/// <param name="minutes">Number of minutes</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddMinutes(Int32 minutes) {
			if (minutes < 0)
			{
				Ticks -= (-minutes) * MinutesTicksCount;
			}
			else
			{
				Ticks += minutes * MinutesTicksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds hours (if negative - substracts)
		/// </summary>
		/// <param name="hours">Number of hours</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddHours(Int32 hours) {
			if (hours < 0)
			{
				Ticks -= (-hours) * HourTicksCount;
			}
			else
			{
				Ticks += hours * HourTicksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds days (if negative - substracts)
		/// </summary>
		/// <param name="days">Number of days</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddDays(Int32 days) {
			if (days < 0)
			{
				Ticks -= (-days) * DayTicksCount;
			}
			else
			{
				Ticks += days * DayTicksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds weeks (if negative - substracts)
		/// </summary>
		/// <param name="weeks">Number of weeks</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddWeeks(Int32 weeks) {
			if (weeks < 0)
			{
				Ticks -= (-weeks) * WeekTicksCount;
			}
			else
			{
				Ticks += weeks * WeekTicksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds months (if negative - substracts)
		/// </summary>
		/// <param name="months">Number of months</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddMonths(Int32 months) {
			if (months < 0)
			{
				Ticks -= (-months) * MonthTicksCount;
			}
			else
			{
				Ticks += months * MonthTicksCount;
			}

			return *this;
		}

		/// <summary>
		/// Adds years (if negative - substracts)
		/// </summary>
		/// <param name="years">Number of years</param>
		/// <returns>New object value</returns>
		ForceInline constexpr TimeSpan& AddYears(Int32 years) {
			if (years < 0)
			{
				Ticks -= (-years) * YearTicksCount;
			}
			else
			{
				Ticks += years * YearTicksCount;
			}

			return *this;
		}

	public:
		/// <summary>
		/// Returns the number of milliseconds
		/// </summary>
		/// <returns>Number of milliseconds</returns>
		ForceInline constexpr Double GetMiliseconds() const {
			return this->Ticks / (MiliSecondsTicksCount * 1.0);
		}

		/// <summary>
		/// Returns the number of seconds
		/// </summary>
		/// <returns>Number of seconds</returns>
		ForceInline constexpr Double GetSeconds() const {
			return this->Ticks / (SecondTicksCount * 1.0);
		}

		/// <summary>
		/// Returns the number of minutes
		/// </summary>
		/// <returns>Number of minutes</returns>
		ForceInline constexpr Double GetMinutes() const {
			return this->Ticks / (MinutesTicksCount * 1.0);
		}

		/// <summary>
		/// Returns the number of hours
		/// </summary>
		/// <returns>Number of hours</returns>
		ForceInline constexpr Double GetHours() const {
			return this->Ticks / (HourTicksCount * 1.0);
		}

		/// <summary>
		/// Returns the number of days
		/// </summary>
		/// <returns>Number of days</returns>
		ForceInline constexpr Double GetDays() const {
			return this->Ticks / (DayTicksCount * 1.0);
		}

		/// <summary>
		/// Returns the number of weeks
		/// </summary>
		/// <returns>Number of weeks</returns>
		ForceInline constexpr Double GetWeeks() const {
			return this->Ticks / (WeekTicksCount * 1.0);
		}

		/// <summary>
		/// Returns the number of months
		/// </summary>
		/// <returns>Number of months</returns>
		ForceInline constexpr Double GetMonths() const {
			return this->Ticks / (MonthTicksCount * 1.0);
		}

		/// <summary>
		/// Returns the number of years
		/// </summary>
		/// <returns>Number of years</returns>
		ForceInline constexpr Double GetYears() const {
			return this->Ticks / (YearTicksCount * 1.0);
		}

	public:
		/// <summary>
		/// Number of milliseconds
		/// </summary>
		Double Property(GetMiliseconds) Miliseconds;

		/// <summary>
		/// Number of seconds
		/// </summary>
		Double Property(GetSeconds) Seconds;

		/// <summary>
		/// Number of minutes
		/// </summary>
		Double Property(GetMinutes) Minutes;

		/// <summary>
		/// Number of hours
		/// </summary>
		Double Property(GetHours) Hours;

		/// <summary>
		/// Number of days
		/// </summary>
		Double Property(GetDays) Days;

		/// <summary>
		/// Number of weeks
		/// </summary>
		Double Property(GetWeeks) Weeks;

		/// <summary>
		/// Number of months
		/// </summary>
		Double Property(GetMonths) Months;

		/// <summary>
		/// Number of years
		/// </summary>
		Double Property(GetYears) Years;

	public:
		/// <summary>
		/// Conversion in date interval
		/// </summary>
		ForceInline constexpr operator DateSpan() const {
			UInt64 delta = 0;

			UInt64 yearsCount = Ticks / YearTicksCount;
			delta += yearsCount * YearTicksCount;

			UInt64 monthsCount = (Ticks - delta) / MonthTicksCount;
			delta += monthsCount * MonthTicksCount;

			UInt64 weeksCount = (Ticks - delta) / WeekTicksCount;
			delta += weeksCount * WeekTicksCount;

			UInt64 daysCount = (Ticks - delta) / DayTicksCount;
			delta += daysCount * DayTicksCount;

			UInt64 hoursCount = (Ticks - delta) / HourTicksCount;
			delta += hoursCount * HourTicksCount;

			UInt64 minutesCount = (Ticks - delta) / MinutesTicksCount;
			delta += minutesCount * MinutesTicksCount;

			UInt64 secondsCount = (Ticks - delta) / SecondTicksCount;
			delta += secondsCount * SecondTicksCount;

			UInt64 milisecodsCount = (Ticks - delta) / MiliSecondsTicksCount;
			UInt64 ticksCount = Ticks % MiliSecondsTicksCount;

			return DateSpan(
				static_cast<UInt16>(yearsCount),
				static_cast<UInt16>(monthsCount),
				static_cast<UInt16>(weeksCount),
				static_cast<UInt16>(daysCount),
				static_cast<UInt16>(hoursCount),
				static_cast<UInt16>(minutesCount),
				static_cast<UInt16>(secondsCount),
				static_cast<UInt16>(milisecodsCount),
				static_cast<UInt16>(ticksCount)
			);
		}

	public:
		/// <summary>
		/// Returns the number of ticks
		/// </summary>
		/// <returns>Number of ticks</returns>
		ForceInline constexpr UInt64 GetTicks() const {
			return this->Ticks;
		}


	public:
		/// <summary>
		/// Checking the equality of two time segments
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>"True" if segments are equal, else "false"</returns>
		ForceInline constexpr bool operator==(TimeSpan other) const { return this->Ticks == other.Ticks; }

		/// <summary>
		/// Checking the inequality of two time segments
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>"True" if segments are not equal, else "false"</returns>
		ForceInline constexpr bool operator!=(TimeSpan other) const { return this->Ticks != other.Ticks; }

		/// <summary>
		/// Comparing of two segments
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Comparing result</returns>
		ForceInline constexpr auto operator<=>(TimeSpan other) const { return this->Ticks <=> other.Ticks; }

	public:
		/// <summary>
		/// Multiplying time by a number, increases the interval by a specified number of times
		/// </summary>
		/// <param name="number">Multiplier</param>
		/// <returns>Resulted value</returns>
		template<Concepts::UnsignedInteger T>
		constexpr TimeSpan operator*(T number) { return TimeSpan(this->Ticks * number); }

		/// <summary>
		/// Dividing time by a number, decreases the interval by a specified number of times
		/// </summary>
		/// <param name="number">Divider</param>
		/// <returns>Resulted value</returns>
		template<Concepts::UnsignedInteger T>
		constexpr TimeSpan operator/(T number) { return TimeSpan(this->Ticks / number); }

		/// <summary>
		/// Multiplying time by a number, increases the interval by a specified number of times
		/// </summary>
		/// <param name="number">Multiplier</param>
		/// <returns>Resulted value</returns>
		template<Concepts::UnsignedInteger T>
		friend constexpr TimeSpan operator*(T number, TimeSpan time) { return TimeSpan(time.Ticks * number); }

		/// <summary>
		/// Dividing time by a number, decreases the interval by a specified number of times
		/// </summary>
		/// <param name="number">Divider</param>
		/// <returns>Resulted value</returns>
		template<Concepts::UnsignedInteger T>
		friend constexpr TimeSpan operator/(T number, TimeSpan time) { return TimeSpan(time.Ticks / number); }

	public:
		/// <summary>
		/// Multiplying the current time object by a number, increases the interval by a specified number of times
		/// </summary>
		/// <param name="number">Multiplier</param>
		/// <returns>Current object</returns>
		template<Concepts::UnsignedInteger T>
		constexpr TimeSpan& operator*=(T number) { this->Ticks *= number; return *this; }

		/// <summary>
		/// Dividing the current time object by a number, decreases the interval by a specified number of times
		/// </summary>
		/// <param name="number">Divider</param>
		/// <returns>Current object</returns>
		template<Concepts::UnsignedInteger T>
		constexpr TimeSpan& operator/=(T number) { this->Ticks /= number; return *this; }

	public:
		StructCttiDef(TimeSpan,
			DefStructFieldData(TimeSpan, Ticks)
		);
	};

	/// <summary>
	/// Time class optimized for a fast arithmetic
	/// </summary>
	struct DateTime final {
	public:
		UInt64 Ticks = 0;

	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		ForceInline constexpr DateTime() = default;

		/// <summary>
		/// Constructor with a set of 100 nanosecond intervals
		/// </summary>
		ForceInline constexpr DateTime(UInt64 time) :Ticks(time) {};

		/// <summary>
		/// Copying constructor
		/// </summary>
		ForceInline constexpr DateTime(const DateTime& orig) :Ticks(orig.Ticks) {};

		/// <summary>
		/// Addition operator to a current time
		/// </summary>
		/// <param name="time">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr DateTime& operator+=(TimeSpan time) { this->Ticks += time.Ticks; return *this; }

		/// <summary>
		/// Substraction operator of a current time
		/// </summary>
		/// <param name="time">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr DateTime& operator-=(TimeSpan time) { this->Ticks -= time.Ticks; return *this; }

		/// <summary>
		/// Addition operator
		/// </summary>
		/// <param name="time1">Left operand</param>
		/// <param name="time2">Right operand</param>
		/// <returns>Operation result</returns>
		ForceInline constexpr friend DateTime operator+(DateTime time1, TimeSpan time2) { return time1.Ticks + time2.Ticks; }

		/// <summary>
		/// Substraction operator
		/// </summary>
		/// <param name="time1">Left operand</param>
		/// <param name="time2">Right operand</param>
		/// <returns>Operation result</returns>
		ForceInline constexpr friend TimeSpan operator-(DateTime time1, DateTime time2) { return time1.Ticks - time2.Ticks; }

		/// <summary>
		/// Substraction operator
		/// </summary>
		/// <param name="time1">Left operand</param>
		/// <param name="time2">Right operand</param>
		/// <returns>Operation result</returns>
		ForceInline constexpr friend TimeSpan operator-(DateTime time1, TimeSpan time2) { return time1.Ticks - time2.Ticks; }

		/// <summary>
		/// Returns the number of 100 nanosecond intervals passed since January 1, 1600
		/// </summary>
		/// <returns>Number of 100 nanosecond intervals passed since January 1, 1600</returns>
		ForceInline constexpr UInt64 GetMsFrom1600() const volatile { return Ticks; }

		/// <summary>
		/// Sets the time as a number of 100 nanosecond intervals passed since January 1, 1600
		/// </summary>
		/// <param name="AbsoluteTime">Number of 100 nanosecond intervals passed since January 1, 1600</param>
		ForceInline constexpr void SetMsFrom1600(UInt64 AbsoluteTime) volatile { this->Ticks = AbsoluteTime; }

		/// <summary>
		/// Number of 100 nanosecond intervals passed since January 1, 1600
		/// </summary>
		UInt64 Property(GetMsFrom1600, SetMsFrom1600) TimeFromStartEpoch;

		/// <summary>
		/// Returns the number of 100 nanosecond intervals in a relative format
		/// </summary>
		/// <returns>Number of 100 nanosecond intervals in a relative format</returns>
		ForceInline constexpr UInt64 GetAsRelativeTime() const volatile { return -static_cast<Int64>(Ticks); }

		/// <summary>
		/// Number of 100 nanosecond intervals in a relative format
		/// </summary>
		UInt64 Property(GetAsRelativeTime) RelativeTime;

		/// <summary>
		/// Return new DateTime with equal system time
		/// </summary>
		/// <returns>DateTime with equal system time</returns>
		DateTime ConvertLocalToSystemTime();

		/// <summary>
		/// Return new DateTime with equal local time
		/// </summary>
		/// <returns>DateTime with equal local time</returns>
		DateTime ConvertSystemToLocalTime();

		/// <summary>
		/// Returns the current system time in Greenwich Mean Time
		/// </summary>
		/// <returns>Current system time in Greenwich Mean Time</returns>
		static DateTime GetCurrentSystemTimeData();

		/// <summary>
		/// Returns the current local time in the time zone of the user or system
		/// </summary>
		/// <returns>Current local time in the time zone of the user or system</returns>
		static DateTime GetCurrentLocalTimeData();

		/// <summary>
		/// Converting constructor
		/// </summary>
		friend Date::Date(const DateTime& time);

		/// <summary>
		/// Converting constructor
		/// </summary>
		friend DateSpan::DateSpan(const DateTime& time);

		/// <summary>
		/// Appropriation operator
		/// </summary>
		/// <param name="newValue">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr DateTime& operator=(DateTime newValue) {
			this->Ticks = newValue.Ticks; return *this;
		}

		/// <summary>
		/// Appropriation operator
		/// </summary>
		/// <param name="newValue">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr volatile DateTime& operator=(const volatile DateTime& newValue) volatile { this->Ticks = newValue.Ticks; return *this; }

	public:
		StructCttiDef(DateTime,
			DefStructFieldData(DateTime, Ticks)
		);
	};

	/// <summary>
	/// Time range
	/// </summary>
	struct TimeRange final {
	public:
		/// <summary>
		/// Range start
		/// </summary>
		DateTime StartTime;

		/// <summary>
		/// Duration
		/// </summary>
		TimeSpan Duration;

	public:
		/// <summary>
		/// An empty constructor, initializes the empty time range
		/// </summary>
		ForceInline constexpr TimeRange() : StartTime(), Duration() {}

		/// <summary>
		/// Constructor with start and duration of time range
		/// </summary>
		/// <param name="startTime">Time range start</param>
		/// <param name="duration">Duration of time range</param>
		ForceInline constexpr TimeRange(DateTime startTime, TimeSpan duration) : StartTime(startTime), Duration(duration) {}

		/// <summary>
		/// Constructor with start and end of time range
		/// </summary>
		/// <param name="startTime">Time range start</param>
		/// <param name="endTime">Time range end</param>
		ForceInline constexpr TimeRange(DateTime startTime, DateTime endTime) : StartTime(0ULL), Duration(0ULL) {
			if (startTime.Ticks == endTime.Ticks)
			{
				this->StartTime = startTime;
				this->Duration = TimeSpan(0);

			}
			else if (startTime.Ticks < endTime.Ticks)
			{
				this->StartTime = startTime;
				this->Duration = TimeSpan(endTime.Ticks - startTime.Ticks);
			}
			else
			{
				this->StartTime = endTime;
				this->Duration = TimeSpan(startTime.Ticks - endTime.Ticks);
			}
		}

		/// <summary>
		/// Constructor with start and duration of time range
		/// </summary>
		/// <param name="startTime">Time range start</param>
		/// <param name="duration">Duration of time range</param>
		ForceInline constexpr TimeRange(DateTime startTime, DateSpan duration) : StartTime(startTime), Duration(duration) {}

	public:
		/// <summary>
		/// If the range of zero length
		/// </summary>
		/// <returns>The logical value</returns>
		ForceInline constexpr Boolean GetIsZeroRange() const { return Duration.Ticks == 0; }

		/// <summary>
		/// If the range of zero length
		/// </summary>
		Boolean Property(GetIsZeroRange) IsZeroRange;

	public:
		StructCttiDef(DateTime,
			DefStructFieldData(TimeRange, StartTime),
			DefStructFieldData(TimeRange, Duration)
		);
	};
}

#pragma warning(push)
//Disabled warnings using user defined literals
#pragma warning(disable: 4455)
#pragma warning(disable: 4114)

/// <summary>
/// Returns the time range in the specified milliseconds
/// </summary>
/// <param name="time">Number of milliseconds</param>
/// <returns>Time range object</returns>
export ForceInline  constexpr Cardinal::Time::TimeSpan operator "" ms(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(MiliSecondsTicksCount * time);
}

/// <summary>
/// Returns the time range in the specified seconds
/// </summary>
/// <param name="time">Number of seconds</param>
/// <returns>Time range object</returns>
export ForceInline  constexpr Cardinal::Time::TimeSpan operator "" s(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(SecondTicksCount * time);
}

/// <summary>
/// Returns the time range in the specified minutes
/// </summary>
/// <param name="time">Number of minutes</param>
/// <returns>Time range object</returns>
export ForceInline constexpr Cardinal::Time::TimeSpan operator "" m(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(MinutesTicksCount * time);
}

/// <summary>
/// Returns the time range in the specified hours
/// </summary>
/// <param name="time">Number of hours</param>
/// <returns>Time range object</returns>
export ForceInline constexpr Cardinal::Time::TimeSpan operator "" h(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(HourTicksCount * time);
}

/// <summary>
/// Returns the time range in the specified days
/// </summary>
/// <param name="time">Number of days</param>
/// <returns>Time range object</returns>
export ForceInline constexpr Cardinal::Time::TimeSpan operator "" d(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(DayTicksCount * time);
}

/// <summary>
/// Returns the time range in the specified months
/// </summary>
/// <param name="time">Number of months</param>
/// <returns>Time range object</returns>
export ForceInline constexpr Cardinal::Time::TimeSpan operator "" M(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(MonthTicksCount * time);
}

/// <summary>
/// Returns the time range in the specified weeks
/// </summary>
/// <param name="time">Number of weeks</param>
/// <returns>Time range object</returns>
export ForceInline constexpr Cardinal::Time::TimeSpan operator "" W(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(WeekTicksCount * time);
}

/// <summary>
/// Returns the time range in the specified years
/// </summary>
/// <param name="time">Number of years</param>
/// <returns>Time range object</returns>
export ForceInline constexpr Cardinal::Time::TimeSpan operator "" Y(const unsigned long long time) {
	using namespace Cardinal::Time;
	return TimeSpan(YearTicksCount * time);
}

#pragma warning(pop)