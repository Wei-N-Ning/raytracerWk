# Exam2 notes

following the failure of Exam1 I made another attempt:

test_render_take2.cpp

this one had a much more pleasing outcome and I managed to complete all the challenges in the book.

I have documented the lessons learned on Jira and in the source code.

The most important take-away is this the high level type-driven design:

High level concepts and their articulation

```text
image driver : feed pixel position to
   camera : generate rays for
      for each ray 
         renderer() **render** ray
            generate hit record or discard
            if (to scatter)
                **render** the scattered ray with attenuation -> recurse into renderer()
            else
                absorb ray and return 0 0 0
            
image driver : color post processing
   for each pixel
      apply LUT (gamma correction)

image driver : output
```

I have also extracted some useful algorithms into gems. Check out `test2_math.hh`
