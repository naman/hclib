include $(HCLIB_ROOT)/include/hclib.mak

FLAGS=-g -std=c++11

nbody: nbody_vec4_main.cu nbody_vec4.cu nbody.h vec4f.h
	nvcc -O3 -rdc=true -arch=sm_35 ${FLAGS} $(PROJECT_CFLAGS) $(PROJECT_LDFLAGS) \
		nbody_vec4.cu nbody_vec4_main.cu -o nbody $(PROJECT_LDLIBS)

clean:
	rm -f nbody
