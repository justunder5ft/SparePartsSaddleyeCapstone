""" This file will be used to test the model in action mostly for the purpose
of seeing how much power it will consume while on the trail.

The script captures a photo every 5 seconds, runs that photo through the trained model, and returns a
trail type that it suspects it is on. while doing it will measure the power consumption on the battery.

We should get the power consumption on a 'per picture' basis as well as how much it takes over 15 minutes, 30 minutes
and so on. We should also do this in many different environments if possible: hot, cold, day, night (though i suspect
that will not have a HUGE effect)."""

