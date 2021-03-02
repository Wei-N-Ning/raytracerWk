# Ray tracing on a weekend (book 1)

I initially followed the book's implementation (chapter by chapter) and wrote
unit tests for each chapter's goal. The code looks highly similar to
those in the book.

Later on I decided to testify my understanding by re-implementing the demos
using my own approach (data structure and algorithms), which were
documented in exam1, 2 and 3.

Exam1 tried to use immutable data structure but lacked of a clear algorithm. It failed.

Exam2 used type-driven development and sketched out the high level algorithm
first. It then implemented each individual `function` in the algorithm. This 
was a good victory of FP. The code was highly reusable and I ended up doing
some look-dev.

Exam3 followed the victorious FP approach and explored parallelism.

Another thing to note is, I don't strictly follow the src-test split rule in these "exam" targets. Their
tests (except for the math primitive unit tests) are next to the implementation file.
