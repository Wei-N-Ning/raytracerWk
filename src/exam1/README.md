# Exam1 Notes

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
