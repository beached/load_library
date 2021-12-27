LoadLibrary abstracts the OS dependent portion of loading shared libraries at runtime.



```c++
auto lib = daw::system::library_handle( library_path );
lib.call_function<ResultType>( function_name, args... );
lib.call_function<ResultType2>( function_name2, args2... );
```
or for one shot calls
```c++
auto result = daw::system::call_library_function<int>( "/path/to/library/file.so", "add", 5, 6 );
```

