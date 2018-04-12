function createFormData() {
    inputs = ['5bi', '4bi', '3bi', '2bi', 'order', 'bi_method', 'output',
              'transform', 'formula', 'polinom', 'nullor', 'Dublicates', 'loop'];

    var fd = new FormData();

    var file = document.getElementById('upload_file').files[0];
    fd.append("file_contents", file);

    for (i in inputs) {
        var element = document.getElementsByName(inputs[i]);

        if (element.length == 1) {
            fd.append(inputs[i], element[0].value);
        } else if (element.length == 2 && element.type == 'radio') {
            to_append = Math.max(element[0].value, element[1].value);
            fd.append(inputs[i], to_append);
        }
    }
    return fd;
}

function request() {
    var client = new XMLHttpRequest();
    var fd = createFormData();

    client.open("post", "http://intersyn.net/api/fetcher.php?action=feed");
    client.send(fd);

    client.onreadystatechange = function()
    {
        if (client.readyState == 4 && client.status == 200)
        {
            var resp = client.responseText;
            resp = resp.substring(resp.indexOf("{"));

            var Jresp = JSON.parse(resp);
            if(Jresp.status = "OK"){
                var urn = Jresp.urn;
            }else{
                var urn = null;
            }

            console.log(urn+".out");
            client.open('GET', urn+".out");
            client.onreadystatechange = function() {
                var div = document.getElementById('output');

                var content = client.responseText;
                div.innerHTML = content;
            }
            client.send();

        }
    }
}

