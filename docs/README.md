# What is Piper?

Piper is an experimental language written by a web developer who is frustrated with the state of "web development languages." The concrete design is still pretty squishy, but there are overarching design goals. Many of them are variations on "make web languages more like systems languages."

## Terminology

I will be using some terms that do not have 100% solid definitions. Therefore, I will attempt to explain where I draw the line.

A "Systems Language" is generally designed to write long-running backend processes. The focus of the language is to make things "correct," often forcing the developer to do extra work to achieve that. Classic examples would be C or Java. You often have to declare types explicitly. You often have fairly accurate knowledge about how memory is laid out, and can optimize your code with that knowledge in mind without really worrying about version-to-version differences.

A "Web Language" is generally designed to write backend processes that handle HTTP requests. The focus of these languages tend to be on developer speed, and not correctness. You are almost never *expected* to annotate types, and in fact in the majority of languages in this category, you cannot (or when you can, they are closer to documentation than restrictions). Often you do not have to declare a variable, it just springs into being the first time you reference it. Prominent examples would be PHP, Python, or Ruby.

## Errors Should Be Caught As Early As Possible

Computers are good at catching errors. Most sysystems languages catch a huge variety of errors, especially if you opt into a level of scrutiny higher than the default. Using a C compiler with `-Wall -Werror` is sure to catch quite a few things. 

Most systems languages tie their own hands when it comes to catching errors. The most wide-spread example I can think of is the lack of having to declare a variable. In a huge number of web languages, assigning to a variable will declare it if it was not already declared. This means that a typo in code will have potentially very subtle errors. Take, for example the following:

    $inches = $input_value;
    if ($input_units == 'feet') {
        $inchs *= 12;
    }

In this very contrived example, there will be a bug any time someone wants to use feet rather than inches. But the bug won't cause a compile-time error. Nor will it even error on this line. It may not "error" at all. But the value will definitely be wrong. It may take a lot of time to notice the bug. The erroenous values may be stored in a database before you notice. You may have to manually try to clean the data based on guesswork and heuristics.

This could have all been avoided if the compiler could notice that `inchs` wasn't declared.

Note that while I am not afraid to force developers to be explicit, that doesn't necessarily mean that developers should have to be explicit about **everything**. **IF** the language can be non-ambiguous and errors can be caught early, I am fine with any and all developer shortcuts. But when developer convenience bumps up against error detection, I will lean toward error detection far more than convenience.

## Memory Management Is Unimportant...

I do not want to have to call malloc and free in my web code. I don't want to have to deal with NULL pointers, or accessing memroy after it is freed. Some form of automatic memory management / garbage collection is necessary.

## .. Until It Is Important

There have been a number of cases where I have needed to interact with binary data in a web language. The API for that case is always pretty annoying, because it's so far down on the priority list of these languages. I want to make sure that I keep this in mind as design things so that I can make the binary APIs not terribly painful to use.

## Strings Are THE Most Important Data Type

If this belief wasn't here, I would probably just use a systems language and be done with it. Systems languages often either ignore their string type, or they have a string type that is fairly mediocre. Part of this pain is caused by things not directly related to strings -- stuff like memory management. But the language needs to be designed to make working with strings as convenient as possible. Splitting, joining, translating, search and replace, all of those and more need to be trivial to do. Without worrying about memory managent or generating lot of garbage. And without having to use a StringBuilder-style API.

## Data Has Interrelated Views

The database representation of data is related to the Object model of data is related to the JSON representation of data is related to the POST variables used to create said data. Web programming often involves translating between these forms, which is full of boilerplate and error-prone. A language that acknowledges that the same logical object has many forms could potentially reduce this problem.

## Data Has Trust Levels

Data that comes from the user is inherently untrusted. Data that comes from the app itself is inherently trusted. Data that comes from the database is somewhere inbetween. Almost every language treats them as the same.

## Some Data Is Shared

Some web languages have zero sharing between two different requests (ie, PHP). Some web languages have sharing allowed, but have complicated semantics around how to actually take advantage of that (Java). In memory data stores are too useful to give up, but it should not be complicated to use them for simple common use cases.

## Code Organization Is Paramount

Maintainability is directly related to how organized code is. Most web languages have very weak organizational primitives, because they are designed to make it easy to write hello world, and forced organizational elements obscure things in such a simple case. But you will want them as things get bigger, and having them be weak or non-existant will come back to bite you. Forcing them in even the simple cases allows them to be baked further into the language, which provides great benefits.

## Third Party Packages Are Important

In an application of any size, you will be using third party libraries. Using them should be convenient. As should finding the packages to use. This goes somewhat outside of language design, but it may have some interactions, and those should be kept in mind.

## Web Languages Can Be Fast

There is a very real divide over web languages and systems languages. Web languages tend to be "interpreted" or close to it. That is, while they might compile to a bytecode, they eschew a separate compilation step, which makes several things notably harder. I will not rule out having a separate compilation step, though it's far from known at this point.

## Single Entry Point

Code is easier to reason about with a single entry point. That entry point shouldn't **do** much, mostly set up configuration, but having a single well-defined entry point makes it easier for new developers to get up to speed.

## Interop With C(-like)

For languages that compile to a library, you can link the libraries together and write different parts of your app in different languages. This seems unimportant (I would never use two different languages in one project, that's just a maintenance nightmare!) until you start thinking about system libraries. To use opencv in PHP, for example, I need to find or write a wrapper around opencv using a pretty complex API that is easy to get wrong. And that API is likely to shift in different versions of PHP.

Since the C ABI is the stable one, with literal decades of libraries being written against it, we should adapt to the C ABI rather than making our own. We aim for automatically being able to use shared libraries without having to manually write a glue layer.

## Generics

Generics are useful. And there's no reason they should be kept from web languages.

## Named Parameters

## Multiple Returns

## Async

Yes. It's awesome.
