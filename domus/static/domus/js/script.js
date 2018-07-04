$(function()
{
    update();

    function update()
    {
      $.get("/domus/update/",
      function(data)
       {

         if(data.valeur)
         {
           console.log(data);
           $('#voyant').css('background-color', 'green');
           event.preventDefault();
         }
         else
         {
           console.log(data);
           $('#voyant').css('background-color', 'red');
           event.preventDefault();
         }
       });
    }

    setInterval(update, 5000);


    //////////////////////////////////////////////
    // MAIN ROOM
    //////////////////////////////////////////////
    // LAMPS
    $('#lum-on-main').click
    (
      function ()
      {
        $.post("/domus/update/",{lum-main: 1});
        event.preventDefault();
      }
    );

    $('#lum-off-main').click
    (
      function ()
      {
        $.post("/domus/update/",{lum-main: 0});
        event.preventDefault();
      }
    );

    $('#lum-percent-main').change
    (
      var message = $('#lum-percent-main').val();
      function ()
      {
        $.post("/domus/update/",{lum-percent-main: message});
        event.preventDefault();
      }
    );


    // PLUG
    $('#plug-on-main').click
    (
      function ()
      {
        $.post("/domus/update/",{plug-main: 1});
        event.preventDefault();
      }
    );

    $('#plug-off-main').click
    (
      function ()
      {
        $.post("/domus/update/",{plug-main: 0});
        event.preventDefault();
      }
    );


    //////////////////////////////////////////////
    // BATHROOM
    //////////////////////////////////////////////
    // LAMPS
    $('#lum-on-bath').click
    (
      function ()
      {
        $.post("/domus/update/",{lum-bath: 1});
        event.preventDefault();
      }
    );

    $('#lum-off-bath').click
    (
      function ()
      {
        $.post("/domus/update/",{lum-bath: 0});
        event.preventDefault();
      }
    );

    $('#lum-percent-bath').change
    (
      var message = $('#lum-percent-bath').val();
      function ()
      {
        $.post("/domus/update/",{lum-percent-bath: message});
        event.preventDefault();
      }
    );

    // PLUG
    $('#plug-on-bath').click
    (
      function ()
      {
        $.post("/domus/update/",{plug-bath: 1});
        event.preventDefault();
      }
    );

    $('#plug-off-bath').click
    (
      function ()
      {
        $.post("/domus/update/",{plug-bath: 0});
        event.preventDefault();
      }
    );

    // HEATING
    $('#lum-on-bath').click
    (
      function ()
      {
        $.post("/domus/update/",{lum-bath: 1});
        event.preventDefault();
      }
    );

    $('#lum-off-bath').click
    (
      function ()
      {
        $.post("/domus/update/",{lum-bath: 0});
        event.preventDefault();
      }
    );

    $('#lum-percent-bath').change
    (
      var message = $('#heating-percent-bath').val();
      function ()
      {
        $.post("/domus/update/",{heating-percent-bath: message});
        event.preventDefault();
      }
    );










    // CONTROL
    $('#ON').click
    (
      function ()
      {
        $.post("/domus/update/",{valeur: 1});
        event.preventDefault();
      }
    );

    $('#OFF').click
    (
      function ()
      {
        $.post("/domus/update/",{valeur: 0});
        event.preventDefault();
      }
    );


    $('#voyant').click
    (
      function ()
      {
        $.get("/domus/update/",
        function(data)
         {

           if(data.valeur)
           {
             console.log(data);
             $('#voyant').css('background-color', 'green');
             event.preventDefault();
           }
           else
           {
             console.log(data);
             $('#voyant').css('background-color', 'red');
             event.preventDefault();
           }

         });
      });
});
