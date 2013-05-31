---
layout: default
title: elaps'd
---

What does elaps'd do for me?
============================

Suppose the following C++ code:

{% highlight c++ linenos %}
#include <omp.h>
#define N 1024

int main(void) {

    double x[N], y[N], z[N];
    int i;

    // Do initializations
    initialize(x, y, z, N);

    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        z[i] = x[i] + y[i];
    }

    printResult(z);

    return 0;
}
{% endhighlight %}

Alright, this works and adds nicely a vector of size `N`. So far, so good. But
performance is everything, right? For instance assume, you want to know how long
the loop runs and possibly also how long each thread computes. These are your
possibilities:

- Use some profiling tool like *gprof* or *perf*. This is going to work, no
  question but the information you get might be too much and you'll have to dig
  into its output ask yourself who calls whom and when and so on. 
- Use a function for timing, e.g. `omp_get_wtime`, which you have to call twice
  and then handle its return value. This is quite easy, but if you also want to
  get detailed thread informations you might be alone with placing calls and
  merging outputs and so on.
- Use something like Intel's *VTune*. Well, this works (if you have a license
  for it), but it might be an overkill.

Another point is, that you might want some graphical analysis, which you won't
get with *gprof* or `omp_get_wtime`.

>*You:* Isn't there an easy-to-use tool for this purpose?

Yes, there is. And this tool is **elaps'd**. To make a measurement, we slightly
extend the code:

{% highlight c++ linenos %}
#include <omp.h>
#define N 1024

#include <elapsd/elapsd.h> //< Include this

int main(void) {

    double x[N], y[N], z[N];
    int i;

    // Do initializations
    initialize(x, y, z, N);

    /* Initialize elaps'd */
    ENHANCE::elapsd e("vector.db", "Vector Experiment");
    e.addKernel(0, "Vector (Multithreaded)");
    e.addDevice(0, "CPU");
    /* fin */

    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        e.startTimer(0,0); //< Magic happens here
        z[i] = x[i] + y[i];
        e.stopTimer(0,0); //< And here
    }

    printResult(z);

    e.commitToDB(); //< Store for analysis

    return 0;
}
{% endhighlight %}

That's all, now compile, link against `libelapsd.so` and run it. After the run
you get a SQLite3 database `vector.db` which you can analyze. Analysis looks
like this:

![alt text](images/elapsd.png)

Cool, isn't it? *elaps'd* automatically recognizes threads when its inside a
multi-threaded environment (e.g. *in* the `for`-loop). Furthermore, the
seperation between *kernels* and *devices* lets you clearly define which
function (kernel) runs on which device. Thus, also measuring functions on
accelerators, e.g. GPGPU, becomes possible -- but currently only on a coarse
level, i.e. you won't see GPGPU threads in the analysis.

In addition to that, multiple *experiments* are supported. That means, you are
able to compare the same program with different arguments. To recognize this
during analysis, *elaps'd* enables you to arbitrarily choose names for
experiments, kernels and devices.
