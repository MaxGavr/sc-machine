### Common

SCs-code - is a text representation of SC-code. Whole text consist of sentences, that
separated by `;;` symbols.

You can use C-style comments in SCs text:
```cpp
// one line comment
fruit -> apple;
/* Multiline
 * comment
 */
```

To make an sc-link into specified file you can use special type identifier:
```
"file://<file name>"
```

* `"file://<file name>"` - is a relative path to a file. According to a file, where it used;
* `"file:///<file_name>"` - is an absolute path to a file.

There are some tricks with object names:

* `...` - is an unnamed object;
* `_<object name>` - all object names, that starts with symbol `_` represents a variable objects.

Objects identifier visibility. By default all objects with name `x` are visible anywhere.
After translating it into memory this object will have a **system identifier** equal to `x`.
So if you use `x` in different *scs* files, then you designate the same object in them
(would be the same element in a knowledge base).

Sometimes you need to designate the same objects in different files, but do not generate a
**system identifier** in memory for it. In this case you should to prefix it name with a `.` symbol.
For example: `.x`.

In case, when you need to make a named object just local for a *scs* file,
then you should to use `..` prefix (example: `..x`).

<div class="note">
SCs-code is splitted into levels. Each level allows to minimize number
of string symbols to represent the same structures.
</div>

### SCs level 1

SCs-code level 1 - is a simple representation of SC-code. It represents it
with just simple triples. Each triple contains `subject`, `predicate`, `object`
that are splitted by `|` symbol. Line `subject | predicate | object;;` is a sentence.

Identifier of `subject`, `predicate`, `object` build with rule:

```
<type>#<identifier>
```

Where `type` - is an element type specification. It can be one of possible values:

* `sc_node` - equal to ![SCg-node](../images/scg/scg_node.png);
* `sc_link` - equal to ![SCg-link](../images/scg/scg_link.png);
* `sc_edge_dcommon` - equal to ![SCg-edge](../images/scg/scg_edge_common_orient.png);
* `sc_edge_ucommon` - equal to ![SCg-edge](../images/scg/scg_edge_common.png);
* `sc_edge_main` - equal to ![SCg-edge](../images/scg/scg_edge_const_pos_perm.png);
* `sc_edge_access` - equal to ![SCg-edge](../images/scg/scg_edge_access.png).

<hr/>

**Examples**

<table>
  <tr>
    <th>SCg construction</th>
    <th>Equal SCs-level 1 text</th>
  </tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_1.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
// append set of apples into fruit set
sc_node#fruit | sc_edge_main#... | sc_node#apple;;
// append set of bananas into fruit set
sc_node#fruit | sc_edge_main#... | sc_node#banana;;
        </code>
      </pre>
    </td>
  </tr>

  <tr>
    <td width="50%"><img src="../../images/scs/scs_example_level_1_2.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
sc_node#apple | sc_edge_dcommon#..e | "file://apple.png";;
/*append edge from nrel_image relation into
  edge between apple set and it's image*/
sc_node#nrel_image | sc_edge_main#... | sc_edge_dcommon#..e;;
        </code>
      </pre>
    </td>
  </tr>
</table>

SCs-level 1 allows you to represent any kind of SC-code construction. It's a low-level
representation and commonly used as a transport format, that is very simple for parsing.

### SCs level 2

This level of SCs-text add two new features:

* using of extended alphabet of edges;
* using of compound identifiers of an edges.

On this level you can make sentences by rule:
```
<element> <edge> <element>;;
```

Where `<edge>` can be on of values:
<table>
  <tr>
    <th>SCs</th>
    <th>SCg equivalent</th>
  </tr>
  <tr>
    <td>`<` or `>`</td>
    <td><img src="../../images/scg/scg_edge_common_orient.png"></img></td>
  </tr>
  <tr>
    <td>`<>`</td>
    <td><img src="../../images/scg/scg_edge_common.png"></img></td>
  </tr>
  <tr>
    <td>`<..` or `..>`</td>
    <td><img src="../../images/scg/scg_edge_access.png"></img></td>
  </tr>
  <tr>
    <td>`<=>`</td>
    <td><img src="../../images/scg/scg_edge_const_common.png"></img></td>
  </tr>
  <tr>
    <td>`_<=>`</td>
    <td><img src="../../images/scg/scg_edge_var_common.png"></img></td>
  </tr>
  <tr>
    <td>`<=` or `=>`</td>
    <td><img src="../../images/scg/scg_edge_const_common_orient.png"></img></td>
  </tr>
  <tr>
    <td>`_<=` or `_=>`</td>
    <td><img src="../../images/scg/scg_edge_var_common_orient.png"></img></td>
  </tr>
  <tr>
    <td>`<-` or `->`</td>
    <td><img src="../../images/scg/scg_edge_const_pos_perm.png"></img></td>
  </tr>
  <tr>
    <td>`_<-` or `_->`</td>
    <td><img src="../../images/scg/scg_edge_var_pos_perm.png"></img></td>
  </tr>
  <tr>
    <td>`<|-` or `-|>`</td>
    <td><img src="../../images/scg/scg_edge_const_neg_perm.png"></img></td>
  </tr>
  <tr>
    <td>`_<|-` or `_-|>`</td>
    <td><img src="../../images/scg/scg_edge_var_neg_perm.png"></img></td>
  </tr>
  <tr>
    <td>`</-` or `-/>`</td>
    <td><img src="../../images/scg/scg_edge_const_fuz_perm.png"></img></td>
  </tr>
  <tr>
    <td>`_</-` or `_-/>`</td>
    <td><img src="../../images/scg/scg_edge_var_fuz_perm.png"></img></td>
  </tr>
  <tr>
    <td>`<~` or `~>`</td>
    <td><img src="../../images/scg/scg_edge_const_pos_temp.png"></img></td>
  </tr>
  <tr>
    <td>`_<~` or `_~>`</td>
    <td><img src="../../images/scg/scg_edge_var_pos_temp.png"></img></td>
  </tr>
  <tr>
    <td>`<|~` or `~|>`</td>
    <td><img src="../../images/scg/scg_edge_const_neg_temp.png"></img></td>
  </tr>
  <tr>
    <td>`_<|~` or `_~|>`</td>
    <td><img src="../../images/scg/scg_edge_var_neg_temp.png"></img></td>
  </tr>
  <tr>
    <td>`</~` or `~/>`</td>
    <td><img src="../../images/scg/scg_edge_const_fuz_temp.png"></img></td>
  </tr>
  <tr>
    <td>`_</~` or `_~/>`</td>
    <td><img src="../../images/scg/scg_edge_var_fuz_temp.png"></img></td>
  </tr>
</table>

<hr/>

<table>
  <tr>
    <th>SCg construction</th>
    <th>Equal SCs-level 2 text</th>
  </tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_1.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
fruit -> apple;;
// backward direction
banana <- fruit;;
        </code>
      </pre>
    </td>
  </tr>
</table>

<hr/>
Compound identifier of an edge builds as a sentence in SCs-level 2, but without
`;;` separator and inside brackets `()`: `(<element> <edge> <element>)`.
So that allows to simplify usage of an edge as a source or target of another one.

<hr/>

**Examples**
<table>
  <tr>
    <th>SCg construction</th>
    <th>Equal SCs-level 2 text</th>
  </tr>

  <tr>
    <td width="50%"><img src="../../images/scs/scs_example_level_1_2.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
nrel_image -> (fruit => "file://apple.png");;
        </code>
      </pre>
    </td>
  </tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_2.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
d -> (c -> (a -> b));;
        </code>
      </pre>
    </td>
  </tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_2_2.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
(a -> b) -> (c <- d);;
        </code>
      </pre>
    </td>
  </tr>

</table>

### SCs level 3

This level of SCs-text allows to minimize symbols to represent constructions like this one:
![SCg-example](../images/scs/scs_example_level_3.png)
```js
c -> (a -> b);;
```

To do that you should use sentence like this:
```
<object> <edge> <attribute>: <object>
```

For this example it would be like this:
```js
a -> c: b;;
```

In case, when output edge from `c` is an variable, then use `::` splitter instead of `:`:
```js
a -> c:: b;;
```
equal to:
```js
c _-> (a -> b);;
```

<div class="note">
<b>Note</b>: you can use <code>:</code>, <code>::</code> just to replace <code>-></code> or <code>_-></code> edges.
</div>

<hr/>

**Examples**
<table>
  <tr>
    <th>SCg construction</th>
    <th>Equal SCs-level 3 text</th>
  </tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_1_2.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
apple => nrel_image: "file://apple.png";;
        </code>
      </pre>
    </td>
  </tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_3_2.png"></img></td>
    <td>
      <div class="note">
        <b>Note</b>: it is possible to use any number of <code>:</code>, <code>::</code> in one sentence.
      </div>
      <pre>
        <code class="js hljs javascript">
a -> c: d:: b;;
        </code>
      </pre>
    </td>
  </tr>
</table>

### SCs level 4

This level of SCs-text allows to combine many setences with one element into one.
For that pupropses used ';' symbol. So if we have some sentences like:
```js
x -> y;;
x <- z;;
x => h: r;;
```

Then using SCs level 4 we can write them like this:
```js
x -> y;
  <- z;
  => h: r;;
```

Another words, this level of SCs text, allows to use source element one time.

<hr/>

**Examples**
<table>
  <tr>
    <th>SCg construction</th>
    <th>Equal SCs-level 4 text</th>
  </tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_1.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
fruit -> apple;
      -> banana;;
        </code>
      </pre>
    </td>
  <tr>

  <tr>
    <td><img src="../../images/scs/scs_example_level_4.png"></img></td>
    <td>
      <pre>
        <code class="js hljs javascript">
a -> c: d: b;
  -> e;
  -> g: f;;
        </code>
      </pre>
    </td>
  </tr>
<table>

### SCs level 5

### SCs level 6
