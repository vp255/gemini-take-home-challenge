Vincent Phan

How to run the program
   With Docker installed, execute: cat sample_input.txt | ./run.sh, or run ./test.sh to run unit tests

How I approached the problem:

   Questions I asked myself in order:
   
     - What data structures should I use?
     
        - use std::map for orders
        
        - use unordered map for the book listing, (could have used std::map which also would keep our instrument to book organized when printing resting orders, but would incur cost, so I chose to use std::unordered_map)
        
     - How to make this performant and modular?
     
        - Add templating for sides, which also reduces code
        
        - Factor out types and utility functions
        
     - Code style
     
        - use structure bindings, explicit casting, and move semantics to make
          things clearer and more performant

   Testing
   
      - mainly tested public interfaces
      
      - considered testing private methods, but considering they are
        abstracted away, less priority than public methods for users of the code
        
      - opted to not test parser failure class considering assumptions I made

   Assumptions 
   
     - "tick data" is correct and well formatted. if submitting other values (EOF, garbage data, unformated data)
       our orderbooks populate with garbage data
       
     - orderids won't be repeated between orders, although this does not
       affect functionality since we do not have to cancel or modify orders, so functionality is preserved

How long I spent:

    Thinking about the code - 1 hr
   
    Writing up first revision 2 hours
   
    Figuring out docker - 1 hour
   
    Customizing make file - 2.5 hours (huge time sink)
   
    Self Code review - 1 hour
   
    Making changes - 1 hour
   
    Writing tests - 2 hours
