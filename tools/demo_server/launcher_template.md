# Demo

<script>
  document.write(
    '<form action="http://' + window.location.hostname + ':4202" method="get">'
  );
</script>
  <table>
  <tr>
    <td>
      The C++ standard to use:
    </td>
    <td>
      <select name="_std">
        <option value="c++11">c++11</option>
        <option value="c++1y">c++14</option>
      </select>
    </td>
  </tr>
  <tr class="separator">
    <td colspan="2">&nbsp;</td>
  </tr>
  #for $config in $configs
    <tr>
      <td>
        The headers of the following
        #if $config.url == ''
          $config.display_name
        #else
          <a href="$config.url">$config.display_name</a>
        #end if
        version will be available:
      </td>
      <td>
        #if $config.name == 'metashell'
        <select name="_branch">
        #else
        <select name="${config.name}_version">
        #end if
        #for version in $config.versions
          #if $version == $config.default_version
            <option selected="selected" value="$version">$version</option>
          #else
            <option value="$version">$version</option>
          #end if
        #end for
        </select>
      </td>
    </tr>
  #end for
  </table>

  <br />

  <input type="submit" value="Start Metashell" />
</form>

