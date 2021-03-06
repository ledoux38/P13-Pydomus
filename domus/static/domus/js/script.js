$(function()
{
    update();

    function update()
    {
      $.get("/domus/update/",
      function(data)
       {
         var arr = data.valeur.digital;
         for (var i = 5; i < arr.length; i++)
         {
           var n = "#" + i.toString();
           var iNum = parseInt(arr[i]);

           if(iNum == 1)
          {
             $(n).css('background-color', 'green');
             event.preventDefault();
          }
          else
          {
             $(n).css('background-color', 'red');
             event.preventDefault();
          }
         }
         var arr = data.valeur.capteurs;
         $('#heatingHumidityBath').val(arr[1]);
         $('#heatingTemperatureBath').val(arr[0]);
       });
    }

    setInterval(update, 10000);


    //////////////////////////////////////////////
    // MAIN ROOM
    //////////////////////////////////////////////
    // LAMPS
    $('#lumOnMain').click
    (
      function ()
      {
        $.post("/domus/update/",{"type": 1, "element": 9, "valeur": 1});
        // $.post("/domus/update/",{9: 1});
        event.preventDefault();
      }
    );

    $('#lumOffMain').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":1, "element":9, "valeur": 0});
        // $.post("/domus/update/",{9: 0});
        event.preventDefault();
      }
    );

    $('#lumPercentMain').change
    (
      function ()
      {
        var message = $('#lumPercentMain').val();
        $.post("/domus/update/",{"type":2, "element":100, "valeur": message});
        // $.post("/domus/update/",{100: message});
        event.preventDefault();
      }
    );


    // PLUG
    $('#plugOnMain').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":1, "element":8, "valeur": 1});
        // $.post("/domus/update/",{8: 1});
        event.preventDefault();
      }
    );

    $('#plugOffMain').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":1, "element":8, "valeur": 0});
        // $.post("/domus/update/",{8: 0});
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
        $.post("/domus/update/",{"type":1, "element":5, "valeur": 1});
        // $.post("/domus/update/",{5: 1});
        event.preventDefault();
      }
    );

    $('#lumOffBath').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":1, "element":5, "valeur": 0});
        // $.post("/domus/update/",{5: 0});
        event.preventDefault();
      }
    );

    $('#lumPercentBath').change
    (
      function ()
      {
        var message = $('#lumPercentBath').val();
        $.post("/domus/update/",{"type":2, "element":110, "valeur": message});
        // $.post("/domus/update/",{110: message});
        event.preventDefault();
      }
    );

    // PLUG
    $('#plugOnBath').click
    (
      function ()
      {
        // $.post("/domus/update/",{6: 1});
        $.post("/domus/update/",{"type":1, "element":6, "valeur": 1});
        event.preventDefault();
      }
    );

    $('#plugOffBath').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":1, "element":6, "valeur": 0});
        // $.post("/domus/update/",{6: 0});
        event.preventDefault();
      }
    );

    // HEATING
    $('#heatingOnBath').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":3, "element":7, "valeur": 1});
        // $.post("/domus/update/",{7: 1});
        event.preventDefault();
      }
    );

    $('#heatingOffBath').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":3, "element":7, "valeur": 0});
        // $.post("/domus/update/",{7: 0});
        event.preventDefault();
      }
    );

    $('#heatingPercentBath').change
    (

      function ()
      {
        var message = $('#heatingPercentBath').val();
        $.post("/domus/update/",{"type":2, "element":120, "valeur": message});
        // $.post("/domus/update/",{120: message});
        event.preventDefault();
      }
    );

    $('#deactivate_all').click
    (
      function ()
      {
        $.post("/domus/update/",{"type":3, "element":130, "valeur": 0});
        // $.post("/domus/update/",{130: 0});
        event.preventDefault();
      }
    );
});
