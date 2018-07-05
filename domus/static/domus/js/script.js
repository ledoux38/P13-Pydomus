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
    $('#lumOnMain').click
    (
      function ()
      {
        $.post("/domus/update/",{9: 1});
        event.preventDefault();
      }
    );

    $('#lumOffMain').click
    (
      function ()
      {
        $.post("/domus/update/",{9: 0});
        event.preventDefault();
      }
    );

    $('#lumPercentMain').change
    (
      function ()
      {
        var message = $('#lumPercentMain').val();
        $.post("/domus/update/",{100: message});
        event.preventDefault();
      }
    );


    // PLUG
    $('#plugOnMain').click
    (
      function ()
      {
        $.post("/domus/update/",{8: 1});
        event.preventDefault();
      }
    );

    $('#plugOffMain').click
    (
      function ()
      {
        $.post("/domus/update/",{8: 0});
        event.preventDefault();
      }
    );


    //////////////////////////////////////////////
    // BATHROOM
    //////////////////////////////////////////////
    // LAMPS
    $('#lumOnBath').click
    (
      function ()
      {
        $.post("/domus/update/",{5: 1});
        event.preventDefault();
      }
    );

    $('#lumOffBath').click
    (
      function ()
      {
        $.post("/domus/update/",{5: 0});
        event.preventDefault();
      }
    );

    $('#lumPercentBath').change
    (
      function ()
      {
        var message = $('#lumPercentBath').val();
        $.post("/domus/update/",{110: message});
        event.preventDefault();
      }
    );

    // PLUG
    $('#plugOnBath').click
    (
      function ()
      {
        $.post("/domus/update/",{6: 1});
        event.preventDefault();
      }
    );

    $('#plugOffBath').click
    (
      function ()
      {
        $.post("/domus/update/",{6: 0});
        event.preventDefault();
      }
    );

    // HEATING
    $('#heatingOnBath').click
    (
      function ()
      {
        $.post("/domus/update/",{7: 1});
        event.preventDefault();
      }
    );

    $('#heatingOffBath').click
    (
      function ()
      {
        $.post("/domus/update/",{7: 0});
        event.preventDefault();
      }
    );

    $('#heatingPercentBath').change
    (

      function ()
      {
        var message = $('#heatingPercentBath').val();
        $.post("/domus/update/",{120: message});
        event.preventDefault();
      }
    );

    $('#deactivate_all').click
    (
      function ()
      {
        $.post("/domus/update/",{130: 0});
        event.preventDefault();
      }
    );
    // CONTROL
    // $('#ON').click
    // (
    //   function ()
    //   {
    //     $.post("/domus/update/",{7: 1});
    //     event.preventDefault();
    //   }
    // );

    // $('#OFF').click
    // (
    //   function ()
    //   {
    //     $.post("/domus/update/",{7: 0});
    //     event.preventDefault();
    //   }
    // );
});
