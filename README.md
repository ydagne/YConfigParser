# YConfigParser
A simple configuration parser for C++

This is a simple configuration parser written in C++. The configuration supports
basic types (bool, int, float, and string) which can be scalar or array.

The configuration format is hierarchical where every sub-field is preceeded with
indentation white-space (or TABS). All sub-fields belonging to the same parent
should have equal indentation. Every parameter-value pair is separated with a colon.
A field might not be followed by a value after the colon, in which case sub-fields
are expected to exist. Moreover, a field might have a value and one or more sub-fields.

Note:

	- Parameters names can be made up of multiple words separated by white spaces.
	- Indentations can be with white spaces or TABs. TABS between white spaces (or vice versa) are not allowed.
	- Every TAB in indentations is counted as one white space.

 The following are list of supported field types:

| **Type**   |   **Example**     |   **Remarks**              | 
|------------|-------------------|----------------------------|
| boolean    |     TRUE          |   TRUE/FALSE in UPPER CASE | 
| int        |     134245        |                            | 
| float      |     15.2453       |                            | 
| string     |     "hello world" |    in double quotes        | 

 In addition to the above basic types, values can be arrays enclosed in squre brackets ([...])
 in which case subsequent entries are separated by a comma (,).
