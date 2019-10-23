/*I like to use 'const <dtype> <class>_<name>' over '#define <tag> <subst>'
  allegedly the first method creates a static object in memory with a fixed value
  where the later type is a macro which could create multiple copies, taking up 
  extra space in our precious memory.  Even if it also creates a constant reference, 
  it is an anonymous type chosen by the compiler.

  This is also provide an anchor for version 3 which will move global variables into
  a namespace or class but more on that later.
*/

 /*  I have used multiple blocks of public and private for member variables and 
  *   functions.  I've chosen to put public variables at the top and private at the
  *   bottom for quickest reference. This is a purely aesthetic choice.
  *   
  *   I am using public variables due to the limited memory available. 
  *   Normally we would consider a getter/setter function pair to a private 
  *   member variable.  The setter sanitizes inputs and a const getter 
  *   protects against accidental writes.
  */

/* Functions and classes.  The 328 only has 2K of active memory available to your
 *  entire program.  It is easy enough to consume this with user defined variables
 *  before we even start to consider the cost of function calls and classes. 
 *  Each function has an address, space for any pass-by-value parameters and so on.
 *  Inherited functions from classes can hide more function calls by creating dynamic
 *  type conversions.  
 *  
 *  That said, debugging the interplay of hardware and software can be challenging. 
 *  My preferred methodology is for both faucets to contain logical, discrete, 
 *  easily testable sub-systems. It is important to remember these are aesthetic 
 *  choices tailored to my way of working. They are not set in stone rules and not 
 *  even always the way I work.  If the passion takes you, write straight ahead with linear
 *  code, then come back in the next version and refactor to be tidier.  Keep
 *  in mind features you wanted to add, but decided to hold off on in V1.  Ask yourself, how
 *  can you make it easier to plug in those ideas without losing the functionality you
 *  have?
 *  
 *  For a direct example, display_pix() felt like a nice place to break a small conept out
 *  of loop(). By breaking the post to hardware into a discrete component it is easier to 
 *  debug or swap out for some other rendering idea.
  */
