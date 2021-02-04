# Exam1 Notes

Exam1 is my experiment to reimplement Peter Shirley's ray tracer using more FP concepts: immutable data structure,
higher order functions and type-driven development.

test_render.cpp

this is my first attempt and fails;

test_render_take2.cpp

this is the second try and succeeds;

I have documented the lessons learned on Jira and in the source code.

The most important take-away is this the high level type-driven design:

High level concepts and their articulation

image driver : feed pixel position to
    camera : generate rays for
        renderer : **render** each ray
            generate hit record or discard
            if (to scatter)
                **render** the scattered ray with attenuation
            else
                absorb ray and return 0 0 0
            
image driver : color post processing
image driver : output

I have also extracted some useful algorithms into gems. Check out `test2_math.hh`
