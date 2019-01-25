## Writing Test Cases
1. Create a source file appended with `-test` just before the `.cpp`
> This is for naming convention sake, the actual test case files don't require `-test` for it to work.
2. Implement a `TEST(arg1, arg2)` function with `arg1` being the Test Suite it belongs in, and `arg2` being the name of the Test Case being executed


![image](/uploads/ac75f6d8d955ac43d96dcbca9a146559/image.png)


> Use `ASSERT` when the condition must hold - if it doesn't the test stops right there. Use this when the
> remainder of the test doesn't have semantic meaning without this condition holding.
>
> Use `EXPECT` when the condition should hold, but in cases where it doesn't we can still get value out of
> continuing the test. (The test will still ultimately fail at the end, though.) [1]


3. Add `{file-name}-test.cpp` to the `{project-root}/testing/CMakeLists.txt` file's `add_executable` function for it to build correcty

![image](/uploads/3455bcb7fa75cb81c2117410569f4d2a/image.png)

Once you've followed these steps, all you need to do is `build` and `make` in your project's out-of-source build directory and you are set.

## Running Tests
To run your new test cases, you need to make sure that you have first `build` and ran `make` from the build directory. A file called `runAllTests` should now exist within the `bin` directory.

To execute every test suites, simply call `{bin_directory}/runAllTests` and you will see something like this:

![image](/uploads/c3dc0e3a41e43a1a94a0e04c9b2674a9/image.png)

[1]: https://stackoverflow.com/questions/2565299/using-assert-and-expect-in-googletest

However, there will be cases where you only want to execute a single test suite. To do this, simply add `--gtest_filter={test_suite_name}*`. This will execute all the test cases within the `{test_suite_name}` namespace.
> To see the list of suites, simply use `--gtest_list_tests`.

## Future Plans
Google Test cases should be executed every time someone compiles the project from the build directory. This merge-request does not contain that function, and will need to be implemented in the future.

Once Sumner releases the exercise for Unit Testing, I will see to it that it automatically executes after the project compiles.