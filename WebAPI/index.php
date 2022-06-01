<?php

require "template.php";

$data = query("SELECT * FROM tabel_kontrol");


if(isset($_POST["simpan"]))  {
    if( tambahOutput($_POST) > 0 ) {

            echo "
                 <script> 
                  Swal.fire({ 
                  title: 'BERHASIL',
                  text: 'Device baru telah disimpan',
                  icon: 'success', buttons: [false, 'OK'], 
                  }).then(function() { 
                      window.location.href='index.php'; 
                  }); 
                 </script>
                ";   
        }
                   
    else {
      echo "
         <script> 
         Swal.fire({ 
            title: 'OOPS', 
            text: 'Device baru gagal disimpan', 
            icon: 'warning', 
            dangerMode: true, 
            buttons: [false, 'OK'], 
            }).then(function() { 
                window.location.href='index.php'; 
            }); 
         </script>
        ";
    }
  } 




?>

<!DOCTYPE html>
  <html>
  <head>
    <meta charset="utf-8">
    <title></title>
    <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css" rel="stylesheet" />
    <link href="css/profile.css" rel="stylesheet" />
    <link href="js/profile.js" rel="javascript" />
  </head>
<body>

<!--  -->
  <center>
    <h4 class="mt-4">RELAY CONTROL PANEL</h4>
    <br>

    <button type="button" class="tambah btn" href="#" style="background:#2E8B57; color:white;"data-toggle="modal"data-target="#tambahOutput"><i class="fa fa-plus"></i> Buat Perangkat Baru</button>
    <a href="gpio.php" class="btn btn-primary"><i class="fa fa-cog"></i> INFO PIN GPIO ESP32/ESP8266</a>

        <div class="table table-responsive-sm mt-3">
            <table class="table table-bordered table-hover table-striped" style="width:48rem;">
               <tr class="bg-dark text-white text-center">
                  <th>Saklar</th>
                  <th>Perangkat</th>
                  <th>Board</th>
                  <th>GPIO</th>
                  <th>Type</th>
                  <th>Log Waktu Pembuatan Data</th>
                  <th>Aksi Data</th>
               </tr>
               <?php foreach ($data as $row) :?>
                 <tr>
                    <td class="text-center"> 
                      <?php 
                      // Active High
                      if ($row["TYPE"] == "Active High") {
                           if($row["STATE"] == 0) { //unchecked = OFF
                               echo '<input type="checkbox" id ='.$row["ID"].' onchange="sendData(this)" data-toggle="toggle" data-onstyle="primary" data-offstyle="danger">';
                             }
                            else { //checked = ON
                                echo '<input type="checkbox" checked id ='.$row["ID"].' onchange="sendData(this)" data-toggle="toggle" data-onstyle="primary" data-offstyle="danger">';
                            }
                      }
                      // Active Low
                      if ($row["TYPE"] == "Active Low") {
                           if($row["STATE"] == 1) { //unchecked = OFF
                               echo '<input type="checkbox" id ='.$row["ID"].' onchange="sendData(this)" data-toggle="toggle" data-onstyle="primary" data-offstyle="danger">';
                             }
                            else  { //checked = ON
                                echo '<input type="checkbox" checked id ='.$row["ID"].' onchange="sendData(this)" data-toggle="toggle" data-onstyle="primary" data-offstyle="danger">';
                            }
                      }
                        

                      ?>
                    </td>
                    <td><?=$row["DEVICE"];?></td>
                    <td class="text-center"><?=$row["BOARD"];?></td>
                    <td class="text-center"><?=$row["GPIO"];?></td>
                    <td class="text-center"><?=$row["TYPE"];?></td>
                    <td class="text-center"><?=$row["LOGTIME"];?></td>
                    <td class="text-center">
                       <a class="ubah btn btn-success btn-sm" href="ubahdevice.php?ID=<?=$row["ID"];?>"><i class="fa fa-edit"></i></a>
                       <a class="hapus btn btn-danger btn-sm alert_hapus" href="hapusOutput.php?ID=<?=$row["ID"];?>"><i class="fa fa-trash"></i></a>
                    </td>
                 </tr>
               <?php endforeach; ?>
            </table>
        </div>

        <!-- Modal Tambah Device -->
<div class="modal fade" id="tambahOutput" tabindex="-1" role="dialog">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">BUAT PERANGKAT RELAY BARU</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <form action="index.php" method="post">
         <div class="modal-body">
              <div class="form-group">
                  <input class="form-control" name="DEVICE" type="text" autocomplete="off" placeholder="Device Name"><br>
                  <input class="form-control" name="BOARD" type="text" autocomplete="off" placeholder="Board" required><br>
                  <input class="form-control" name="GPIO" type="text" autocomplete="off" placeholder="GPIO" required><br>
                  <p>TIPE KELUARAN GPIO:</p>
                  <div class="row px-6">
                            <div class="col">
                              <div class="form-check">
                                <input class="form-check-input" type="radio" name="TYPE" value="Active High" required>
                                <label class="form-check-label">Aktif Tinggi</label>
                              </div>
                            </div>
                            <div class="col">
                               <div class="form-check">
                                  <input class="form-check-input" type="radio" name="TYPE" value="Active Low" required>
                                  <label class="form-check-label">Aktif Rendah</label>
                              </div>
                            </div>
                          </div>          
            </div>  
      </div>
      <div class="modal-footer">
        <button type="submit" name="simpan" class="btn btn-success"><i class="fa fa-save"></i> Simpan</button>
        <button type="reset" name="reset" class="btn text-white" style="background: blue"><i class="fa fa-sync-alt"></i> Reset</button>
        <button type="button" class=" btn btn-danger" data-dismiss="modal"> <i class="fa fa-undo"></i> Batal</button>
      </div>
     </form>
    </div>
  </div>
</div>

      <script>
          //send data
           function sendData(e){
              var xhr = new XMLHttpRequest();
                  if(e.checked){
                    xhr.open("GET", "proses.php?id="+e.id+"&state= 1", true);
                  }
                  else{
                    xhr.open("GET", "proses.php?id="+e.id+"&state= 0", true);
                  }
                xhr.send();
              }

                function reveal() {
                  var reveals = document.querySelectorAll(".reveal");

                  for (var i = 0; i < reveals.length; i++) {
                    var windowHeight = window.innerHeight;
                    var elementTop = reveals[i].getBoundingClientRect().top;
                    var elementVisible = 360;

                    if (elementTop < windowHeight - elementVisible) {
                      reveals[i].classList.add("active");
                    } else {
                      reveals[i].classList.remove("active");
                    }
                  }
                }

                window.addEventListener("scroll", reveal);

      </script>
<br/>
<strong>Skrol Kebawah Untuk Melihat Tim &#8595;</strong>
<footer class="footer">
<section>
  <div class="container reveal">
    <div class="text-container">
      <div class="text-box">
        <div class="container d-flex justify-content-center py-6">
          <div class="col-12 col-sm-6 col-lg-12">
            
            <!-- Section Heading-->
            <div class="section_heading text-center wow fadeInUp" data-wow-delay="1.2s" style="visibility: visible; animation-delay: 1.2s; animation-name: fadeInUp;">
              <h3>Tim Kreatif MPSRI Kami</h3>
              <hr/>
              <marquee>Kami Mengutamakan: Kereativitas, Kesempurnaan, Fungsitivitas &amp; Kemudahan.</marquee>
              <hr/>
              <div class="line"></div>
            </div>
          </div>
        </div>
        <div class="row">

          <!-- Single Advisor Profile-->
          <div class="col-12 col-sm-3 col-lg-3">
            <div class="single_advisor_profile wow fadeInUp" data-wow-delay="4.2s" style="visibility: visible; animation-delay: 4.2s; animation-name: fadeInUp;">
              <!-- Team Thumb-->
              <div class="advisor_thumb"><img src="img/author3.jpg" alt="">
              </div>
              <!-- Team Details-->
              <div class="single_advisor_details_info">
                <h6>Anjas Amar Pradana</h6>
                <p class="designation">Mentoring &amp; Instruktur</p>
                <hr/>
                <p class="designation">INFO: Hanya Pembantu</p>
              </div>
            </div>
          </div>

          <!-- Single Advisor Profile-->
          <div class="col-12 col-sm-3 col-lg-3">
            <div class="single_advisor_profile wow fadeInUp" data-wow-delay="4.2s" style="visibility: visible; animation-delay: 4.2s; animation-name: fadeInUp;">
              <!-- Team Thumb-->
              <div class="advisor_thumb"><img src="img/author2.jpg" alt="">
              </div>
              <!-- Team Details-->
              <div class="single_advisor_details_info">
                <h6>Rizki Heri Purwanto</h6>
                <p class="designation">Developer Program &amp; Pendukung Sistem</p>
                <hr/>
                <p class="designation">NIM: 19040110</p>
              </div>
            </div>
          </div>

          <!-- Single Advisor Profile-->
          <div class="col-12 col-sm-3 col-lg-3">
            <div class="single_advisor_profile wow fadeInUp" data-wow-delay="4.2s" style="visibility: visible; animation-delay: 4.2s; animation-name: fadeInUp;">
              <!-- Team Thumb-->
              <div class="advisor_thumb"><img src="img/author1.jpg" alt=""></div>
              <!-- Team Details-->
              <div class="single_advisor_details_info">
                <h6>Mohammad Aditya Putra</h6>
                <p class="designation">Mandor &amp; Pendukung Keuangan</p>
                <hr/>
                <p class="designation">NIM: 19040110</p>
              </div>
          </div>
        </div>

      </div>

        </div>
      </div>
    </div>
  </div>
</section>

</footer>

</body>
<br/><br/>
</html> 
