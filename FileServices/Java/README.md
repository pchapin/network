
README
======

Install the latest version of the Ice system from ZeroC (version 3.7.10 at the time of this
writing). Then generate the stubs and skeletons manually by going into the `src` folder and
running the command:

```
slice2java ../../Storage.ice
```

This should create a `storage` subfolder beneath `src` where the generated package contents are
placed. The IntelliJ projects for the Client and Server are configured to know where this
information is located.

If any changes are made to `Storage.ice`, the Slice compiler will need to be rerun (manually).
Right now, IntelliJ does not know how to run it automatically.
