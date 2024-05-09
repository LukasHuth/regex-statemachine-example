# Regex Test

This project had two purposes, the first was to see if i am able to create a simple regex statemachine and the second one was,
how much of an improvement such a statemachine could be in comparison to a simple recursive regex function

## How to run

### Download

```bash
git clone git@github.com:LukasHuth/regex-statemachine-example.git
cd regex-statemachine-example
```

### Building

```bash
make
```

### Running

The program can either be run by typing
```bash
./main
```
to run all tests or
```bash
./main 1 2 3
```
where `1`, `2` and `3` are example indicies of the implemented [tests](#tests)

## Tests

1. regex: `"abc"` value: `"abc"` expected: `true`
2. regex: `"ab."` value: `"abc"` expected: `true`
3. regex: `"a*"` value: `"aaa"` expected: `true`
4. regex: `"a*b"` value: `"aaab"` expected: `true`
5. regex: `"b*"` value: `"aaa"` expected: `false`
6. regex: `".*b.*c.*d.*e.*"` expected: `true`
    - value: `"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaacaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaadaaaaaaaaaaaaaaaaaaaaaaaaacaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaeaaaaaaaaaaaaaaaaaaaaaaaaaaa"`
7. regex: `".*.*a.*"` expected: `true`
    - value: `"abbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"`
